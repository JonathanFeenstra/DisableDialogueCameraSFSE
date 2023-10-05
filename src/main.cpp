class MenuOpenCloseEventSink final : 
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
	public ISingleton<MenuOpenCloseEventSink>
{
	RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
	{
		if (a_event.menuName == "DialogueMenu" && a_event.opening) {
			if (const auto playerCamera = RE::PlayerCamera::GetSingleton(); playerCamera) {
				playerCamera->SetCameraState(RE::CameraState::kFirstPerson);
			}
		}

		return RE::BSEventNotifyControl::kContinue;
	}
};

static void Install()
{
	if (const auto ui = RE::UI::GetSingleton()) {
		ui->RegisterSink(MenuOpenCloseEventSink::GetSingleton());
		logger::info("Registered MenuOpenCloseEventSink");
	}
}

static void MessageCallback(SFSE::MessagingInterface::Message* a_msg) noexcept
{
	switch (a_msg->type) {
	case SFSE::MessagingInterface::kPostLoad:
		Install();
		break;
	default:
		break;
	}
}

DLLEXPORT constinit auto SFSEPlugin_Version = []() noexcept {
	SFSE::PluginVersionData data{};

	data.PluginVersion(Version::MAJOR);
	data.PluginName(Version::PROJECT);
	data.AuthorName("Jonathan Feenstra");
	data.UsesSigScanning(true);
	//data.UsesAddressLibrary(true);
	data.HasNoStructUse(true);
	//data.IsLayoutDependent(true);
	data.CompatibleVersions({ SFSE::RUNTIME_SF_1_7_29 });

	return data;
}();

DLLEXPORT bool SFSEAPI SFSEPlugin_Load(const SFSE::LoadInterface* a_sfse)
{
	SFSE::Init(a_sfse);

	SFSE::GetMessagingInterface()->RegisterListener(MessageCallback);

	return true;
}
