static void Login(bool* p_open)
{
	static char userName[64];
	static char password_Id[64];

	// important configuration for full screen window
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);
	// // // // // // // // // //

	ImGui::Text("This is a voting system for Jordanian citizens.");
	ImGui::NewLine();

	ImGui::Text("Log In");
	ImGui::NewLine();

	ImGui::Text("User name   ");
	ImGui::SameLine();
	ImGui::InputText("##", userName, IM_ARRAYSIZE(userName));

	ImGui::Text("National ID ");
	ImGui::SameLine();
	ImGui::InputText("##password (clear)", password_Id, IM_ARRAYSIZE(password_Id));

	if (ImGui::Button("Sign in"))
	{
		if (p1.LogIn(userName, password_Id) && !loggedIn)
		{
			ImGui::Text("You have successfully logged in , now go to menu tab for inqueires. ");

			loggedIn = true;
		}
		else
		{
			/*  loggedIn = false;*/
			std::cout << "-- wrong username or id .\n";
		}
	}

	if (loggedIn)
	{
		std::string name = "a";
		if (name._Equal(userName))
		{
			adminLogin = true;
		}

		ImGui::Text("You have successfully logged in , now go to menu tab for inqueires. ");

		ImGui::NewLine();
		ImGui::NewLine();
	}
}
static void Menu(bool* p_open)
{
	if (!loggedIn)
	{
		ImGui::Text("You should log in to the system first.");
		return;
	}

	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

	ImGui::Text("Hello ! ");

	ImGui::NewLine();
	ImGui::NewLine();

	if (ImGui::Button("Candidate Registration"))
	{
		candidateRegistrationMode = true;
	}

	ImGui::NewLine();
	if (ImGui::Button("Candidate Status"))
	{
		candidateStatusMode = true;
	}

	ImGui::NewLine();
	if (ImGui::Button("View Candidates "))
	{

	}

	ImGui::NewLine();
	if (ImGui::Button("Vote"))
	{

	}
}