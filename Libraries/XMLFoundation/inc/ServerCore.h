//
// ServerCore.h defines application usable interfaces found in ServerCore.cpp 
//


class GString;  // forward declare

class UserProfile
{
public:
	GString strHashSalt;	 // a random string assigned to this user at account creation - must not contain a triple colon :::
	GString strPassord;		 // if the iterator has a next item    it()     ?   use the next item it++   :    otherwise use ""
	GString strGroups;		 // list of 1 byte permission groups that this user is a member of - u=users r=root 
	GString strUserDetails;  // a sublist with a ; delimiter with comment details about the user such as Full Name, Phone #, and Address.  [Ensure that none of the data content contains a triple colon :::]
	GString strHomeDirectory;// a Path that could apply to some commands	[Ensure that none of the data content contains a triple colon :::]
	GString strEnvironment;  //  could apply to some commands	           [Ensure that none of the data content contains a triple colon :::]

};
int Permission(const char* pzAuthKey, const char* pzCmd, UserProfile* pUser);
bool LoadApplicationProfile(bool bCheckExecutablePath = true, bool bCheckCurrentWorkingDir = true);

// This currently stores users inside the ServerCore GProfile application settings file.  It's similiar in design to the 
// way users are stored in Linux besides the fact that in Linux the passwords file, the shadow file, is separated and only accessable by user root.
// This is a new 2024 development, the interfaces and process is still open to change prior to it being used extensively.
int AddOrUpdateUserAccount(const char* pzUser, const char* pzPassword, const char* pzDetails = 0, const char* pzDirectory = 0, const char* pzEnvironment = 0);
void ExtractUserAndPass(GString* strUser, GString* strPassword, const char* pzAuthKey);

void server_stop();
int server_start(const char* pzStartupMessage = 0, GString* strOutFailedReason = 0);
void viewPorts(GString* pG = 0);
void showActiveThreads(GString* pG/* = 0*/);

