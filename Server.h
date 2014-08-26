//---------------------------------------------------------------------------

#ifndef ServerH
#define ServerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Web.Win.Sockets.hpp>
#include <Data.DB.hpp>
#include <Data.DBXMySQL.hpp>
#include <Data.SqlExpr.hpp>
#include <Data.FMTBcd.hpp>
#include <Xml.Win.msxmldom.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TTcpServer *ServerClient;
	TMemo *Memo1;
	TSQLConnection *SQLConnection;
	TXMLDocument *XML;
	TTcpClient *ServerServer;
	void __fastcall ServerClientAccept(TObject *Sender, TCustomIpClient *ClientSocket);
	void __fastcall FormCreate(TObject *Sender);



private:	// User declarations
	void __fastcall LoadXML();
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
