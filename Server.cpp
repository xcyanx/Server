//---------------------------------------------------------------------------

#include <vcl.h>
#include "Packet.h"
#pragma hdrstop

#include "Server.h"
#include <RegularExpressions.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ServerClientAccept(TObject *Sender, TCustomIpClient *ClientSocket)

{
	LogInPacket lp;
	BasicPacket bp;
	TGUID guid;
	int SQLResult;


	TSQLQuery *SQLQuery = NULL;

	Memo1->Lines->Add("Client connected with IP: "+ClientSocket->RemoteHost);

	if(ClientSocket->WaitForData(5000) == false)
	{
		Memo1->Lines->Add("Error: Client socket could no accept data.");
	}
	else
	{
		ClientSocket->ReceiveBuf(&lp, sizeof(LogInPacket));
	}

	//Check for the packet header.

	if(lp.PacketID == BasicPacket::ACC_VERIFICATION)
	{
			 SQLQuery = new TSQLQuery(NULL);

			 SQLQuery->SQLConnection = SQLConnection;

			 //LogInPacket *logPack = static_cast<LogInPacket*>(&bp);

			//Decrypt the data.



			//Now read from the database the username and the password and send back
			//the response.

			try
			{
				SQLQuery->SQL->Clear();
				SQLQuery->Close();

                SQLQuery->SQL->Add("SELECT * FROM Users WHERE Username = :uname and Password = :upass;");

				SQLQuery->Prepared = true;

				SQLQuery->ParamByName("uname")->AsAnsiString = lp.username;
				SQLQuery->ParamByName("upass")->AsAnsiString = lp.password;

				SQLQuery->Open();

				SQLResult = SQLQuery->RowsAffected;

				SQLQuery->Close();
			}
			catch(TDBXError &e)
			{
			  	Memo1->Lines->Add("SQL Exception: "+e.Message);
			}

			//Finally send the ACC_VERIFIED or -1.
			//If the user exists then use the CoCreateGuid() to generate a guid and send it aswell.
			//And send the guid at the data handler server.

			if(SQLResult == 1)
			{
				bp.PacketID = BasicPacket::ACC_VERIFIED;

				/*if(CoCreateGuid(&guid) != S_OK)
				{
						if(SQLQuery != NULL)
							delete SQLQuery;

						return;
                }

				bp.guid = guid;

				ServerServer->Active = true;

                ServerServer->Open();

				ServerServer->

				ServerServer->Close();

				ServerServer->Active = false;

				Memo1->Lines->Add(String("Sent guid ")+System::Sysutils::GUIDToString(guid)+String(" to ")+ClientSocket->RemoteHost);*/

				Memo1->Lines->Add("IP "+ClientSocket->RemoteHost+" logged in with username "+String(lp.username));
			}
			else
			{
				bp.PacketID = BasicPacket::ACC_NOTVERIFIED;
            }

			ClientSocket->SendBuf(&bp, sizeof(bp));

	}  //Verification
	else if(lp.PacketID == BasicPacket::ACC_CREATE)
	{
		bp.PacketID = BasicPacket::ACC_NOTAVAILABLE;

		ClientSocket->SendBuf(&bp, sizeof(bp));
    }

	if(SQLQuery != NULL)
		delete SQLQuery;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormCreate(TObject *Sender)
{

	Memo1->Lines->Add("Initiating Server.");

	Memo1->Lines->Add("Loading the options file.");

	LoadXML();

	//Memo1->Lines->Add("Initiating the socket for the Server to Server communication.");

	//ServerServer->Active = true;

	Memo1->Lines->Add("Initiating the socket for the Server to Client communication.");

	ServerClient->Active = true;

	Memo1->Lines->Add("Initiating the connection to the database.");

	SQLConnection->Connected = true;

	Memo1->Lines->Add("Server initiation completed.");

	Memo1->Lines->Add("");
}

void _fastcall TForm1::LoadXML()
{
	String path = ExtractFilePath(Application->ExeName);

	XML->FileName = path+(String)"Options.xml";
	XML->Active = true;

	_di_IXMLNodeList root = XML->ChildNodes->GetNode("Options")->GetChildNodes();

	//ServerServer->LocalHost = root->GetNode("ServerToServer")->GetChildNodes()->GetNode("Host")->Text;
	//ServerServer->LocalPort = StrToInt(root->GetNode("ServerToServer")->GetChildNodes()->GetNode("Port")->Text);

	ServerClient->LocalHost = root->GetNode("ServerToClient")->GetChildNodes()->GetNode("Host")->Text;
	ServerClient->LocalPort = StrToInt(root->GetNode("ServerToClient")->GetChildNodes()->GetNode("Port")->Text);

	String uname = root->GetNode("DB")->GetChildNodes()->GetNode("uname")->Text,
		   host = root->GetNode("DB")->GetChildNodes()->GetNode("Host")->Text,
		   pass = root->GetNode("DB")->GetChildNodes()->GetNode("pass")->Text;

	SQLConnection->Params->Add("User_Name="+uname);
	SQLConnection->Params->Add("HostName="+host);
	SQLConnection->Params->Add("Password="+pass);

}
//---------------------------------------------------------------------------





