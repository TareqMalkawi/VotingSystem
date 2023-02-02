#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <imgui/imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Person.hpp"
#include "Render.hpp"
#include "Utility.hpp"

constexpr int MAX_CANDIDATES_NUMBER = 10;

static bool loggedIn;
static bool adminLoggedIn;
static bool candidateRegistrationMode;
static bool candidateStatusMode;
static bool viewCandidatesMode;
static bool voteMode;
static bool electionResultMode;
static bool candidateRegistered;
static bool hasBeenVerified;

static bool readFileOnce;
static bool readValueOnce;
static bool writeCandidateStatementToFileOnce;
static bool readCandidatesElectionStatementsFileOnce;
static bool generateUniqueIdOncePerSession;

static std::vector<std::string> storage;
static std::vector<std::string> candidateVerifiedList;
static std::vector<std::string> candidatesElectionStatements;

static bool candidatesCheckbox[MAX_CANDIDATES_NUMBER];
static bool writeFileOnce[MAX_CANDIDATES_NUMBER];

//static char candidatesNote[MAX_CANDIDATES_NUMBER][24];
static char candidateStatement[1024 * 16];

static int CandidatesNumber;

static int uniqueId;
static std::string uniqueIdString;
static const char* uniqueIdChar;
static bool calculateElectionResult;
static bool showElectionResultsToUsers;

static bool readCandidatesNamesOnce;
static std::vector<std::string> verifiedCandidatesNames;

static bool votedCandidatesNamesList[MAX_CANDIDATES_NUMBER];

static std::string candidatesResults;
static char uniqueIdCharInput[16];
static bool continuedToVote;
static bool displayOfficialCandidatesList;
static bool writeVotesIncreamentOnce;

static std::map<std::string, int> candidatesVotes;
static std::vector<std::string> verifiedCandidatesWithVotesList;
static bool calculateResultOnce;

// static bool readCandidatesNamesOnce1;
static bool readElectionCandidatesNameOnce;

static std::string winnerName;

// temporary variables to store the login data from user.
static char userName[64];
static char password_Id[64];
static std::string userType;

void RenderView::Login(bool* p_open)
{
	// important configuration for full screen window.
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

	//	//	//	//	//

	ImGui::Text("This is a voting system for Jordanian citizens");
	ImGui::Text("Area : Irbid province");
	ImGui::NewLine();

	ImGui::Text("Log in");
	ImGui::NewLine();

	ImGui::Text("User name   ");
	ImGui::SameLine();
	ImGui::InputText("##", userName, IM_ARRAYSIZE(userName));

	ImGui::Text("National ID ");
	ImGui::SameLine();
	ImGui::InputText("##password (clear)", password_Id, IM_ARRAYSIZE(password_Id));

	ImGui::NewLine();
	if (ImGui::Button("Log in"))
	{
		if (Person::Login(userName, password_Id) && !loggedIn)
			loggedIn = true;
		else
			std::cout << "-- wrong username or id .\n";
	}

	if (loggedIn)
	{
		userType = Person::GetUserTypeFromDatabase(password_Id);

		std::string name = "admin";
		if (name._Equal(userName))
		{
			ImGui::Text("You have successfully logged in as an ADMIN.");

			adminLoggedIn = true;
		}
		else
		{
			ImGui::NewLine();
			ImGui::TextWrapped("You have successfully logged in , now you can go to the election menu tab where the election is being conducted.");
		}

		ImGui::NewLine();
		ImGui::NewLine();
	}
}

void RenderView::ElectionMenu(bool* p_open)
{
	if (!loggedIn)
	{
		ImGui::Text("You should log in to the system first");
		return;
	}

	// important configuration for full screen window.
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

	//	//	//	//	//

	ImGui::Text("Welcome to the Jordanian election platform, Irbid province");
	ImGui::NewLine();

	if (userType == "Admin")
	{
		// hide menu tab.
		ImGui::Text("Admin can't see election menu");
		ImGui::NewLine();
	}
	else if (userType == "Candidate")
	{
		ImGui::TextWrapped("Here is the election menu, where you can register as a candidate , vote,...");
		ImGui::NewLine();

		ElectionMenuModes(4);
	}
	else if (userType == "Voter")
	{
		ImGui::TextWrapped("Here is the election menu, where you can view and vote for candidates");
		ImGui::NewLine();

		ElectionMenuModes(2);
	}
}

void RenderView::ElectionMenuModes(short modesNum)
{
	ImGui::TextWrapped("Your unique Id use it to vote, no need to memeroize as its generated once per session");

	if (!generateUniqueIdOncePerSession)
	{
		uniqueId = Person::IdGenerator();
		uniqueIdString = std::to_string(uniqueId);
		uniqueIdChar = uniqueIdString.c_str();

		std::cout << uniqueIdChar<<"\n";

		generateUniqueIdOncePerSession = true;
	}

	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), uniqueIdChar);
	ImGui::NewLine();

	if (modesNum > 2) // candidate / voter menu
	{
		if (ImGui::Button("Candidate Registration"))
		{
			candidateRegistrationMode = true;
		}

		ImGui::NewLine();
		if (ImGui::Button("   Candidate Status   "))
		{
			candidateStatusMode = true;
		}

		ImGui::NewLine();
		if (ImGui::Button("   View Candidates	"))
		{
			viewCandidatesMode = true;
		}

		ImGui::NewLine();
		if (ImGui::Button("         Vote         "))
		{
			voteMode = true;
		}

		ImGui::NewLine();
		if (ImGui::Button("   Election Result    "))
		{
			electionResultMode = true;
		}
	}
	else // voter only menu
	{
		ImGui::NewLine();
		if (ImGui::Button("   View Candidates	"))
		{
			viewCandidatesMode = true;
		}

		ImGui::NewLine();
		if (ImGui::Button("         Vote         "))
		{
			voteMode = true;
		}

		ImGui::NewLine();
		if (ImGui::Button("   Election Result    "))
		{
			electionResultMode = true;
		}
	}
}

void RenderView::CandidateRegistration()
{
	static char userName[64];
	static char nationality[64];
	static char national_Id[64];
	static char age[64];
	static char crimeRecord[64];
	static char education[64];

	static bool isFilled;

	// important configuration for full screen window.
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

	//	//	//	//	//

	ImGui::Text("Fill the follwing form to register as a candidate");
	ImGui::Text("Make sure to fill the whole form");
	ImGui::NewLine();

	ImGui::Text("User name     ");
	ImGui::SameLine();
	ImGui::InputText("##", userName, IM_ARRAYSIZE(userName));

	ImGui::Text("Nationality   ");
	ImGui::SameLine();
	ImGui::InputText("##1", nationality, IM_ARRAYSIZE(nationality));

	ImGui::Text("National ID   ");
	ImGui::SameLine();
	ImGui::InputText("##password (clear)", national_Id, IM_ARRAYSIZE(national_Id));

	ImGui::Text("Age           ");
	ImGui::SameLine();
	ImGui::InputText("##2", age, IM_ARRAYSIZE(age));

	ImGui::Text("Crime         ");
	ImGui::SameLine();
	ImGui::InputText("##3", crimeRecord, IM_ARRAYSIZE(crimeRecord));

	ImGui::Text("Education     ");
	ImGui::SameLine();
	ImGui::InputText("##4", education, IM_ARRAYSIZE(education));


	ImGui::NewLine();
	if (ImGui::Button("Register"))
	{
		Utils::WriteFile("CandidatesData.txt", userName);
		Utils::WriteFile("CandidatesData.txt", nationality);
		Utils::WriteFile("CandidatesData.txt", national_Id);
		Utils::WriteFile("CandidatesData.txt", age);
		Utils::WriteFile("CandidatesData.txt", crimeRecord);
		Utils::WriteFile("CandidatesData.txt", education);

		candidateRegistered = true;
	}

	if (candidateRegistered)
	{
		ImGui::Text("Registered successfully.");
		ImGui::Text("Your application has been sent to the election commitee to check in.");
		ImGui::Text("You can check on your application status in Candidate Status.");
	}
}

void RenderView::CandidateStatus()
{
	Utils::ReadFile("CandidateVerificationList.txt", candidateVerifiedList);

	std::vector<std::string>::iterator itr;
	itr = std::find(candidateVerifiedList.begin(), candidateVerifiedList.end(), password_Id);

	if (itr != candidateVerifiedList.end() && !readValueOnce)
	{
		//std::cout<< itr - candidateVerifiedList.begin()<<"\n";
		std::cout << "Your application has been verified. \n";
		hasBeenVerified = true;
		readValueOnce = true;
	}
	else if (!readValueOnce)
	{
		std::cout << "Your application still pending. check later.\n";
		readValueOnce = true;
	}

	if (hasBeenVerified)
	{
		ImGui::TextWrapped("Congratulations, You have successfully been verified to become an offical candidate");
		ImGui::TextWrapped("Down below you can write your election statement so voters can decide to whom they vote");
		ImGui::NewLine();

		ImGui::InputTextMultiline("##source", candidateStatement, IM_ARRAYSIZE(candidateStatement), ImVec2(-FLT_MIN,
			ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);

		ImGui::NewLine();
		ImGui::Text("Once you finish, please press save");

		ImGui::NewLine();
		if (ImGui::Button("Save Statement") && !writeCandidateStatementToFileOnce)
		{
			// write to file renamed with national id of the candidate.
			std::cout << "your election statement has been saved\n";
			Utils::CreateAndWriteFile(password_Id, candidateStatement);
			writeCandidateStatementToFileOnce = true;
		}
		else
		{
			// std::cout << "Statement hasn't been saved\n";
		}
	}

	if (candidateRegistered)
	{
		ImGui::Text("Your application still pending. check later.");
	}
}

void RenderView::AdminDashboard(bool* p_open)
{
	if (adminLoggedIn)
	{
		ImGui::Text("Admin dashboard");
		ImGui::NewLine();

		if (!readFileOnce)
		{
			Utils::ReadFile("CandidatesData.txt", storage);
			CandidatesNumber = storage.size() / 6;
			readFileOnce = true;
		}

		// display candidates data to screen
		for (int i = 0; i < storage.size(); i++)
		{
			ImGui::Text(storage[i].c_str());
			if ((i + 1) % 6 == 0)
			{
				ImGui::Text("*-----*-----*-----*-----*-----*-----*");
			}
		}
		ImGui::NewLine();

		if (CandidatesNumber > 0)
			HandleVerification(0);

		ImGui::NewLine();
		ImGui::NewLine();
		if (ImGui::Button("   Calculate Election Result  "))
		{
			calculateElectionResult = true;
		}

		if (calculateElectionResult)
		{
			ElectionResult();
		}

		ImGui::NewLine();
		if (ImGui::Button("Show Election Results To Users"))
		{
			const char* showElectionResultsToUsersCondidtion = "1";
			Utils::CreateAndWriteFile("Conditions.txt", showElectionResultsToUsersCondidtion);
			showElectionResultsToUsers = true;
		}

		if (showElectionResultsToUsers)
			DisplayElectionResults();
	}
}

void RenderView::HandleVerification(int index)
{
	std::string candidateVerificationString = " Verify Candidate " + std::to_string(index + 1);
	char const* candidateVerificationChar = candidateVerificationString.c_str();

	std::string candidateNoteString = "##" + std::to_string(index);
	char const* candidateNoteChar = candidateNoteString.c_str();

	ImGui::Checkbox(candidateVerificationChar, &candidatesCheckbox[index]);
	
	// display note to not verified candidates.
	/*ImGui::SameLine();
	ImGui::Text("Note");
	ImGui::SameLine();
	ImGui::InputText(candidateNoteChar, candidatesNote[index], IM_ARRAYSIZE(candidatesNote[1]));*/

	if (candidatesCheckbox[index] && !writeFileOnce[index])
	{
		std::string national_ID = storage[2 + (index * 6)]; // read national ID of each user.
		const char* national_IDChar = national_ID.c_str();

		std::cout << "Candidate with the follwing national ID " << national_ID << " has been Verified \n";
		Utils::WriteFile("CandidateVerificationList.txt", national_IDChar);

		// store and initialize each candidate with 0 votes
		std::string candidateNameFromNationalID = Person::GetUserNameFromDatabase(national_ID);
		std::string candidateVotes = candidateNameFromNationalID + "_" + "0";
		std::string fileName = candidateNameFromNationalID + ".txt";
		std::cout << fileName;
		Utils::CreateAndWriteFile(fileName, candidateVotes.c_str());

		writeFileOnce[index] = true;
	}
	else
	{
		//std::cout << "Candidate has not been Verified \n";
	}

	if (index < (CandidatesNumber - 1))
	{
		HandleVerification(++index);
	}
}

void RenderView::ViewCandidates()
{
	ImGui::Text("Viewing the offical candidates and thier election statements");
	ImGui::NewLine();

	if (!readCandidatesElectionStatementsFileOnce)
	{
		Utils::ReadFile("CandidateVerificationList.txt", candidateVerifiedList);

		for (int i = 0; i < candidateVerifiedList.size(); i++)
		{
			Utils::ReadFile(candidateVerifiedList[i], candidatesElectionStatements);
		}

		readCandidatesElectionStatementsFileOnce = true;
	}

	for (int i = 0; i < candidatesElectionStatements.size(); i++)
	{
		/*ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), Person::GetUserNameFromDatabase(candidateVerifiedList[i]).c_str());
		ImGui::Text("*-----*-----*-----*");
		ImGui::Spacing();*/
		const char* c = candidatesElectionStatements[i].c_str();
		ImGui::Text(c);
		//ImGui::Text("*-----*-----*-----*");
		//ImGui::NewLine();
	}

	ImGui::NewLine();
	ImGui::Text("*-----*-----*-----*");
}

void RenderView::Vote()
{
	// note : voters can't vote till the election start. //  IMPORTANT !

	ImGui::Text("Unique Id   ");
	ImGui::SameLine();
	ImGui::InputText("##UniqueId", uniqueIdCharInput, IM_ARRAYSIZE(uniqueIdCharInput));

	ImGui::SameLine();
	if (ImGui::Button("Continue"))
	{
		continuedToVote = true;
	}

	if (continuedToVote)
	{
		std::cout << std::atoi(uniqueIdCharInput) << "  " << std::atoi(uniqueIdChar) << "\n";
		if (std::atoi(uniqueIdCharInput) == std::atoi(uniqueIdChar))
		{
			std::cout << "correct unique id entered \n";
			displayOfficialCandidatesList = true;
		}
		else
		{
			std::cout << "Wrong unique id entered \n";
		}
		continuedToVote = false;
	}

	if (displayOfficialCandidatesList)
	{
		ImGui::NewLine();
		ImGui::Text("Official Candidates");
		ImGui::NewLine();

		if (!readCandidatesNamesOnce)
		{
			Utils::ReadFile("CandidateVerificationList.txt", candidateVerifiedList);

			for (int i = 0; i < candidateVerifiedList.size(); i++)
			{
				verifiedCandidatesNames.push_back(Person::GetUserNameFromDatabase(candidateVerifiedList[i]));
			}

			readCandidatesNamesOnce = true;
		}

		// std::cout << verifiedCandidatesNames.size();

		ImGui::NewLine();
		for (int i = 0; i < verifiedCandidatesNames.size(); i++)
		{
			ImGui::Text(verifiedCandidatesNames[i].c_str());
			ImGui::SameLine();
			ImGui::Text("*-----*-----*-----*");
			ImGui::SameLine();
			std::string candidateVoteCheckboxString = "##" + std::to_string(i);
			char const* candidateVoteCheckboxChar = candidateVoteCheckboxString.c_str();
			ImGui::Checkbox(candidateVoteCheckboxChar, &votedCandidatesNamesList[i]);
			ImGui::NewLine();

			// if user vote for specific candidate
			if (votedCandidatesNamesList[i] && !writeVotesIncreamentOnce)
			{
				Utils::ReadFile(Person::GetUserNameFromDatabase(candidateVerifiedList[i]) + ".txt", candidatesResults);

				std::string name = candidatesResults.substr(0, candidatesResults.find("_"));
				size_t prevotes = candidatesResults.find("_");
				std::string votesValue = candidatesResults.substr(prevotes + 1);
				std::cout << "before : " << votesValue << "\n";
				int newVotesValue = std::stoi(votesValue) + 1;
				std::cout << "after : " << newVotesValue << "\n";

				std::string temp = name + "_" + std::to_string(newVotesValue);
				Utils::DeleteAndWriteFile(Person::GetUserNameFromDatabase(candidateVerifiedList[i]) + ".txt", temp.c_str());

				writeVotesIncreamentOnce = true;
			}
		}
	}
}

void RenderView::ElectionResult()
{
	if (!readCandidatesNamesOnce)
	{
		Utils::ReadFile("CandidateVerificationList.txt", candidateVerifiedList);

		for (int i = 0; i < candidateVerifiedList.size(); i++)
		{
			verifiedCandidatesNames.push_back(Person::GetUserNameFromDatabase(candidateVerifiedList[i]));
		}

		readCandidatesNamesOnce = true;
	}

	if (!calculateResultOnce)
	{
		if (!readElectionCandidatesNameOnce)
		{
			for (int i = 0; i < verifiedCandidatesNames.size(); i++)
			{
				Utils::ReadFile(verifiedCandidatesNames[i] + ".txt", verifiedCandidatesWithVotesList);
			}

			readElectionCandidatesNameOnce = true;
		}
	
		for (int i = 0; i < verifiedCandidatesWithVotesList.size(); i++)
		{
			std::string name = verifiedCandidatesWithVotesList[i].substr(0, verifiedCandidatesWithVotesList[i].find("_"));
			size_t prevotes = verifiedCandidatesWithVotesList[i].find("_");
			std::string votesValueString = verifiedCandidatesWithVotesList[i].substr(prevotes + 1);
			int votesValue = std::stoi(votesValueString);
			candidatesVotes[verifiedCandidatesNames[i]] = votesValue;
		}
				
		int maxVotes = candidatesVotes[verifiedCandidatesNames[0]];
		for (int i = 1; i < candidatesVotes.size(); i++)
		{
			if (maxVotes < candidatesVotes[verifiedCandidatesNames[i]])
				maxVotes = candidatesVotes[verifiedCandidatesNames[i]];
		}

		std::cout <<"Max votes : " << maxVotes <<"\n";

		// only in C++17
		for (auto& [key, value] : candidatesVotes)
		{
			if (value == maxVotes)
			{
				std::cout << "The winner candidate is  : " << key << "\n";
				Utils::WriteFile("TheWinner.txt", key.c_str());
			}
		}

		calculateResultOnce = true;
		calculateElectionResult = false;
	}	
}

void RenderView::MainView()
{
	// important configuration for full screen window.
	static bool use_work_area = true;
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
	ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

	//	//	//	//	//

	if (ImGui::Begin("Voting System", NULL, flags))
	{
		const char* names[8] = { "Login", "Election Menu", "Candidate Registration", "Candidate Status" , "View Candidates" , "Vote" , "Election Result" , "Admin Dashboard"};
		static bool opened[8] = { true, true, true, true , true , true , true , true }; // Persistent user state
		
		// debug only
		/*for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
		{
			if (n > 0) { ImGui::SameLine(); }
			ImGui::Checkbox(names[n], &opened[n]);
		}*/

		if (ImGui::BeginTabBar("Voting System"))
		{
			if (opened[0] && ImGui::BeginTabItem(names[0], &opened[0], ImGuiTabItemFlags_None))
			{
				Login(NULL);

				ImGui::EndTabItem();
			}

			if (opened[1] && ImGui::BeginTabItem(names[1], &opened[1], ImGuiTabItemFlags_None))
			{
				ElectionMenu(NULL);

				ImGui::EndTabItem();
			}

			if (candidateRegistrationMode)
			{
				if (opened[2] && ImGui::BeginTabItem(names[2], &opened[2], ImGuiTabItemFlags_None))
				{
					CandidateRegistration();

					ImGui::EndTabItem();
				}
			}

			if (candidateStatusMode)
			{
				if (opened[3] && ImGui::BeginTabItem(names[3], &opened[3], ImGuiTabItemFlags_None))
				{
					CandidateStatus();

					ImGui::EndTabItem();
				}
			}

			if (viewCandidatesMode)
			{
				if (opened[4] && ImGui::BeginTabItem(names[4], &opened[4], ImGuiTabItemFlags_None))
				{
					ViewCandidates();
					ImGui::EndTabItem();
				}
			}

			if (voteMode)
			{
				if (opened[5] && ImGui::BeginTabItem(names[5], &opened[5], ImGuiTabItemFlags_None))
				{
					Vote();

					ImGui::EndTabItem();
				}
			}

			if (electionResultMode)
			{
				if (opened[6] && ImGui::BeginTabItem(names[6], &opened[6], ImGuiTabItemFlags_None))
				{
					DisplayElectionResults();

					ImGui::EndTabItem();
				}
			}

			if (adminLoggedIn)
			{
				if (opened[7] && ImGui::BeginTabItem(names[7], &opened[7], ImGuiTabItemFlags_None))
				{
					AdminDashboard(NULL);

					ImGui::EndTabItem();
				}
			}

			ImGui::EndTabBar();
		}
		ImGui::Separator();
	}

	ImGui::End();
}

void RenderView::DisplayElectionResults()
{
	if (!readCandidatesNamesOnce)
	{
		std::string Conditions;
		Utils::ReadFile("Conditions.txt", Conditions);
		if (Conditions.empty())
		{
			ImGui::Text("The election Results are not available at the moment");
			return;
		}
		else
		{
			if (std::stoi(Conditions) == 0)
			{
				ImGui::Text("The election Results are not available at the moment");
				return;
			}
			else
			{
				Utils::ReadFile("CandidateVerificationList.txt", candidateVerifiedList);
				Utils::ReadFile("TheWinner.txt", winnerName);

				for (int i = 0; i < candidateVerifiedList.size(); i++)
				{
					verifiedCandidatesNames.push_back(Person::GetUserNameFromDatabase(candidateVerifiedList[i]));
				}

				readCandidatesNamesOnce = true;
			}		
		}
	}

	if (!readElectionCandidatesNameOnce)
	{
		for (int i = 0; i < verifiedCandidatesNames.size(); i++)
		{
			Utils::ReadFile(verifiedCandidatesNames[i] + ".txt", verifiedCandidatesWithVotesList);
		}

		readElectionCandidatesNameOnce = true;
	}

	for (int i = 0; i < verifiedCandidatesWithVotesList.size(); i++)
	{
		std::string name = verifiedCandidatesWithVotesList[i].substr(0, verifiedCandidatesWithVotesList[i].find("_"));
		size_t prevotes = verifiedCandidatesWithVotesList[i].find("_");
		std::string votesValueString = verifiedCandidatesWithVotesList[i].substr(prevotes + 1);
		
		int votesValue = std::stoi(votesValueString);
		
		std::string nameFollowedByValue = name + "			" + votesValueString;
		ImGui::Text(nameFollowedByValue.c_str());
	}

	ImGui::NewLine();
	ImGui::Text("The winner candidate is : ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),winnerName.c_str());
}