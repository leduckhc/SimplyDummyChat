#pragma once

#include <msclr/marshal_cppstd.h>


namespace CppCLRWinformsProjekt
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace msclr::interop;


	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();

			context = gcnew marshal_context();
			cipher = gcnew AesCipher();
			client = gcnew ChatClient();
		}
		~Form1(void)
		{
			client->Close();
			workerReceiveMessage->CancelAsync();
			//workerReceiveMessage->ReportProgress()			

			delete workerReceiveMessage;

			if (components)
				delete components;
			if (context)
				delete context;
			//context->Dispose();
		}


	private:
		System::ComponentModel::Container^ components;
		System::Windows::Forms::Label^ lblPassword;
		System::Windows::Forms::Label^ lblUsername;
		System::Windows::Forms::Label^ lblHost;
		System::Windows::Forms::TextBox^ tbxPassword;
		System::Windows::Forms::TextBox^ tbxUsername;
		System::Windows::Forms::TextBox^ tbxHost;
		System::Windows::Forms::Button^ btnLogin;
		System::Windows::Forms::TextBox^ txbMessage;
		System::Windows::Forms::RichTextBox^ rtxbMain;
		System::Windows::Forms::Button^ btnSend;
		// protoze nejde prijimat (recv) s timeout, tak vytvorime vlakno
		System::ComponentModel::BackgroundWorker^ workerReceiveMessage;
		marshal_context^ context;  // Conversion of (System::String ^) to ( char*)
		// https://docs.microsoft.com/en-us/troubleshoot/dotnet/csharp/convert-systemstring-char

		ChatClient^ client;
		///*SimpleCipher*/^ cipher;
		AesCipher^ cipher;

		bool hasNewMessage;

#pragma region Windows Form Designer generated code
		   /// <summary>
		   /// Erforderliche Methode für die Designerunterstützung.
		   /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		   /// </summary>
		   void InitializeComponent(void)
		   {
			   this->lblPassword = (gcnew System::Windows::Forms::Label());
			   this->lblUsername = (gcnew System::Windows::Forms::Label());
			   this->tbxPassword = (gcnew System::Windows::Forms::TextBox());
			   this->tbxUsername = (gcnew System::Windows::Forms::TextBox());
			   this->btnLogin = (gcnew System::Windows::Forms::Button());
			   this->txbMessage = (gcnew System::Windows::Forms::TextBox());
			   this->rtxbMain = (gcnew System::Windows::Forms::RichTextBox());
			   this->btnSend = (gcnew System::Windows::Forms::Button());
			   this->workerReceiveMessage = (gcnew System::ComponentModel::BackgroundWorker());
			   this->tbxHost = (gcnew System::Windows::Forms::TextBox());
			   this->lblHost = (gcnew System::Windows::Forms::Label());
			   this->SuspendLayout();
			   // 
			   // lblPassword
			   // 
			   this->lblPassword->AutoSize = true;
			   this->lblPassword->Location = System::Drawing::Point(22, 75);
			   this->lblPassword->Name = L"lblPassword";
			   this->lblPassword->Size = System::Drawing::Size(56, 13);
			   this->lblPassword->TabIndex = 0;
			   this->lblPassword->Text = L"Password:";
			   // 
			   // lblUsername
			   // 
			   this->lblUsername->AutoSize = true;
			   this->lblUsername->Location = System::Drawing::Point(22, 49);
			   this->lblUsername->Name = L"lblUsername";
			   this->lblUsername->Size = System::Drawing::Size(58, 13);
			   this->lblUsername->TabIndex = 1;
			   this->lblUsername->Text = L"Username:";
			   // 
			   // tbxPassword
			   // 
			   this->tbxPassword->Location = System::Drawing::Point(89, 72);
			   this->tbxPassword->Name = L"tbxPassword";
			   this->tbxPassword->Size = System::Drawing::Size(140, 20);
			   this->tbxPassword->TabIndex = 2;
			   // 
			   // tbxUsername
			   // 
			   this->tbxUsername->Location = System::Drawing::Point(89, 46);
			   this->tbxUsername->Name = L"tbxUsername";
			   this->tbxUsername->Size = System::Drawing::Size(140, 20);
			   this->tbxUsername->TabIndex = 3;
			   // 
			   // btnLogin
			   // 
			   this->btnLogin->Location = System::Drawing::Point(145, 98);
			   this->btnLogin->Name = L"btnLogin";
			   this->btnLogin->Size = System::Drawing::Size(75, 23);
			   this->btnLogin->TabIndex = 4;
			   this->btnLogin->Text = L"Login";
			   this->btnLogin->UseVisualStyleBackColor = true;
			   this->btnLogin->Click += gcnew System::EventHandler(this, &Form1::btnLogin_Click);
			   // 
			   // txbMessage
			   // 
			   this->txbMessage->Enabled = false;
			   this->txbMessage->Location = System::Drawing::Point(25, 417);
			   this->txbMessage->Name = L"txbMessage";
			   this->txbMessage->Size = System::Drawing::Size(303, 20);
			   this->txbMessage->TabIndex = 5;
			   // 
			   // rtxbMain
			   // 
			   this->rtxbMain->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			   this->rtxbMain->Location = System::Drawing::Point(25, 134);
			   this->rtxbMain->Name = L"rtxbMain";
			   this->rtxbMain->ReadOnly = true;
			   this->rtxbMain->Size = System::Drawing::Size(403, 260);
			   this->rtxbMain->TabIndex = 6;
			   this->rtxbMain->Text = L"";
			   // 
			   // btnSend
			   // 
			   this->btnSend->Enabled = false;
			   this->btnSend->Location = System::Drawing::Point(353, 414);
			   this->btnSend->Name = L"btnSend";
			   this->btnSend->Size = System::Drawing::Size(75, 23);
			   this->btnSend->TabIndex = 7;
			   this->btnSend->Text = L"Send";
			   this->btnSend->UseVisualStyleBackColor = true;
			   this->btnSend->Click += gcnew System::EventHandler(this, &Form1::btnSend_Click);
			   // 
			   // workerReceiveMessage
			   // 
			   this->workerReceiveMessage->WorkerSupportsCancellation = true;
			   this->workerReceiveMessage->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::workerReceiveMessage_DoWork);
			   this->workerReceiveMessage->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::workerReceiveMessage_RunWorkerCompleted);
			   // 
			   // tbxHost
			   // 
			   this->tbxHost->Enabled = true;
			   this->tbxHost->Location = System::Drawing::Point(89, 20);
			   this->tbxHost->Name = L"tbxHost";
			   this->tbxHost->Size = System::Drawing::Size(140, 20);
			   this->tbxHost->TabIndex = 9;
			   this->tbxHost->Text = L"localhost";
			   // 
			   // lblHost
			   // 
			   this->lblHost->AutoSize = true;
			   this->lblHost->Location = System::Drawing::Point(22, 23);
			   this->lblHost->Name = L"lblHost";
			   this->lblHost->Size = System::Drawing::Size(32, 13);
			   this->lblHost->TabIndex = 8;
			   this->lblHost->Text = L"Host:";
			   // 
			   // Form1
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(451, 462);
			   this->Controls->Add(this->tbxHost);
			   this->Controls->Add(this->lblHost);
			   this->Controls->Add(this->btnSend);
			   this->Controls->Add(this->rtxbMain);
			   this->Controls->Add(this->txbMessage);
			   this->Controls->Add(this->btnLogin);
			   this->Controls->Add(this->tbxUsername);
			   this->Controls->Add(this->tbxPassword);
			   this->Controls->Add(this->lblUsername);
			   this->Controls->Add(this->lblPassword);
			   this->Name = L"Form1";
			   this->Text = L"Chat";
			   this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion
	private:
		System::Void btnLogin_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// Login button triggers Client Socket initialization
			std::string host = context->marshal_as<std::string>(tbxHost->Text);

			
			if (client->Init(host.c_str())) {
				// Disable login inputs
				btnLogin->Enabled = false;
				tbxHost->Enabled = false;
				tbxPassword->Enabled = false;
				tbxUsername->Enabled = false;
				// Enable chat inputs
				txbMessage->Enabled = true;
				btnSend->Enabled = true;
				// Add a message
				//rtxbMain->Text = rtxbMain->Text->Concat(
				//	rtxbMain->Text,
				//	"Trying to log in with username ",
				//	tbxUsername->Text, "\n\n");
				// Start the Background Worker to receive message
				workerReceiveMessage->RunWorkerAsync();

				// Send in the form channel:text
				const char* password = context->marshal_as<const char*>(tbxPassword->Text);
				//  char* msg = context->marshal_as< char*>(tbxUsername->Text);

				// Set key for your cipher
				//cipher->SetKeyfile("keyfile");
				//cipher->SetKey(password);
				cipher->SetKeyfile();

				// Dont cipher welcome text
				client->Send(password);
			}
			else
			{
				MessageBox::Show(L"Failed connecting to server. Start the server before proceeding.");
			}
		}

		System::Void btnSend_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (client->is_closed)
			{
				MessageBox::Show(L"The client is closed.");
				// Allow login buttons
				btnLogin->Enabled = true;
				tbxHost->Enabled = true;
				tbxPassword->Enabled = true;
				tbxUsername->Enabled = true;
				// Disable chat
				txbMessage->Enabled = false;
				btnSend->Enabled = false;
			}
			else if (txbMessage->Text != "")
			{
				// Send button triggers Client Socket to send message

				// Conversion of (System::String ^) to ( char*)
				String^ sMessage = tbxUsername->Text + ": " + txbMessage->Text;
				std::string plainMsg = context->marshal_as<std::string>(sMessage);
				std::string cipherMsg = cipher->Encode(plainMsg);
				if (client->Send(cipherMsg.c_str())) {
					rtxbMain->Text = rtxbMain->Text->Concat(
						rtxbMain->Text, "\n",
						sMessage, "\n");
				}

				//delete[] plainMsg;
				//delete[] cipherMsg;

				txbMessage->Text = "";
			}
		}

		System::Void workerReceiveMessage_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
		{
			if (client->is_closed == false)
			{
				hasNewMessage = false;
				hasNewMessage = client->Receive();
			}

			System::Threading::Thread::Sleep(1000); // nanosecs
		}

		System::Void workerReceiveMessage_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e)
		{
			if (client->is_closed == false)
			{
				if (hasNewMessage)
				{
					char* cipherMsg = client->GetBuffer();
					if (strlen(cipherMsg) > 0)
					{
						std::string plainMsg = cipher->Decode(cipherMsg);
						//System::String^ mmsg = gcnew System::String(plainMsg);
						System::String^ mmsg = context->marshal_as<System::String^>(plainMsg); // gcnew System::String(plainMsg);

						rtxbMain->Text = rtxbMain->Text->Concat(
							rtxbMain->Text, "\n",
							mmsg, "\n");

						//delete[] cipherMsg;
						//delete[] plainMsg;
					}
				}
				else
				{
					MessageBox::Show(L"Incorrect Password.");

					client->Close();
					// Allow login buttons
					btnLogin->Enabled = true;
					tbxHost->Enabled = true;
					tbxPassword->Enabled = true;
					tbxUsername->Enabled = true;
					// Disable chat
					txbMessage->Enabled = false;
					btnSend->Enabled = false;
				}

				// loop the worker
				workerReceiveMessage->RunWorkerAsync(); // znova zavola DoWork(...)
			}
			
		}
	private: System::Void Form1_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
	{
	}
};
}
