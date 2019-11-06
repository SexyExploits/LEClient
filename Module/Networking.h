#include "stdafx.h"

class Networking {
public:
	static VOID DecryptNetworking();
	static VOID Disconnect();
	static BOOL m_Initalized;
	static BOOL Connected;
	static SOCKET m_Socket;
	static DWORD Connect();
	static DWORD Recv(PVOID Data, DWORD BytesExpected);
	static DWORD Send(PACKET_COMMAND CommandID, PVOID Request, DWORD RequestLen, PVOID Responce, DWORD ResponceLength, BOOL KeepConnectionOpen = FALSE, BOOL ShouldRecieve = TRUE);

private:
	static BYTE rgSessionKeySalt[];
	static BYTE rgSessionKeySaltSeed[];
	static BYTE rgSessionKey[];
	static BYTE PrimaryDec[];

	static INT XorMagic(INT Key);
	static VOID BuildNetworkingHeader(PBYTE Data, DWORD DataLen);
	static VOID XorMagic(PBYTE Key);
	static DWORD StartupServerCommunicator();
	static DWORD Send(PACKET_COMMAND CommandID, PVOID Data, DWORD DataLen);
};
