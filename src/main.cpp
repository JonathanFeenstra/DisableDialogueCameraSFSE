namespace Hook
{
	typedef void(WINAPI* pFunc)(std::uint64_t a_1, RE::CameraState);
	inline pFunc originalFunction;
	inline static void replacementFunction(std::uint64_t a_1, RE::CameraState a_cameraState)
	{
		const auto currentState = *stl::adjust_pointer<RE::CameraState>(RE::PlayerCamera::GetSingleton()->currentState, 0x50);
		originalFunction(a_1, a_cameraState);
		if (a_cameraState == RE::CameraState::kDialogue) {
			originalFunction(a_1, currentState);
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

static void MessageCallback(SFSE::MessagingInterface::Message* a_msg) noexcept
{
	switch (a_msg->type) {
	case SFSE::MessagingInterface::kPostLoad:
		Hook::Install();
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
