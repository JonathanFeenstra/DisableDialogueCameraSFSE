namespace Hook
{
	typedef void(WINAPI* pFunc)(std::uint64_t a_1, RE::CameraState);
	inline pFunc originalFunction;

	inline static void replacementFunction(std::uint64_t a_1, RE::CameraState a_cameraState)
	{
		if (a_cameraState != RE::CameraState::kDialogue) {
			originalFunction(a_1, a_cameraState);	
		}
	}

	inline static void Install()
	{
		const auto targetAddress = REL::ID(166078).address();
		originalFunction = (pFunc)targetAddress;
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)originalFunction, (PBYTE)&replacementFunction);
		if (DetourTransactionCommit() == NO_ERROR) {
			logger::info("Installed SetCameraState hook");
		} else {
			logger::warn("Could not install SetCameraState hook");
		}
	}
};

namespace RE // Copied from https://github.com/Bobbyclue/Console-Command-Runner-SF/blob/8a11a81bc88440ad7f9023f406fcbf377e98d75d/src/Hooks.cpp#L5-L19
{
	void* BGSScaleFormManager()
	{
		static REL::Relocation<void**> singleton{ REL::ID(879512) };
		return *singleton;
	}

	void ExecuteCommand(void* a_scaleformManager, const char* a_command)
	{
		using func_t = decltype(&ExecuteCommand);
		REL::Relocation<func_t> func{ REL::ID(166307) };
		func(a_scaleformManager, a_command);
	}
}

namespace EventSink
{
	class MenuOpenCloseEventSink final :
		public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
		public ISingleton<MenuOpenCloseEventSink>
	{
		RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
		{
			if (a_event.menuName == "DialogueMenu" && !a_event.opening) {
				RE::ExecuteCommand(RE::BGSScaleFormManager(), "player.ToggleControlsDriven 1");
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
};

static void MessageCallback(SFSE::MessagingInterface::Message* a_msg) noexcept
{
	switch (a_msg->type) {
	case SFSE::MessagingInterface::kPostLoad:
		Hook::Install();
		EventSink::Install();
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
