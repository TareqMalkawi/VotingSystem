class vector;

class RenderView
{
public:
	static void Login(bool* p_open);
	static void ElectionMenu(bool* p_open);
	static void ElectionMenuModes(short modesNum);
	static void CandidateRegistration();
	static void CandidateStatus();
	static void AdminDashboard(bool* p_open);
	static void HandleVerification(int index);	
	static void ViewCandidates();
	static void Vote();
	static void ElectionResult();
	static void MainView();
	static void DisplayElectionResults();
};