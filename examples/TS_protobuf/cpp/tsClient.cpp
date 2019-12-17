
/*g++ -o tsClient -I. -I /Users/j.murphy/nChain/SDK-CLEAN/sdklibraries-john/src/core -I /Users/j.murphy/nChain/SDK-CLEAN/build/generated/hpp serverConnection.cpp MessageRequestResponse.cpp TSMessageFactory.cpp GroupMetaData.cpp tsClient.cpp ts.pb.cc orchestrator.cpp player.cpp -L /Users/j.murphy/nChain/SDK-CLEAN/build/x64/release -lprotobuf -lboost_system-mt -lboost_thread-mt -lboost_chrono-mt -ldl -lBigNumbers -lMessageHash -lECPoint -lpolynomial -pthread -std=c++17*/


#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <serverConnection.h>
#include <MessageRequestResponse.h>
#include <TSMessageFactory.h>



std::string Options (){
    std::stringstream os;
    os << "List of available options"
        << "\n\t1.....Print the time"
        << "\n\tr.....Register player"
        << "\n\tl.....List Registered players"
        << "\n\tc.....create a group"
        << "\n\tp.....print list of groups the player is a member of"
        << "\n\tg.....list group details for a given group id"
        << "\n\td.....delete group with id (also removes it from the server)"
        << "\n\th.....help (prints this message)"
        << "\n\ts.....share a secret (private key) within a given group"
        << "\n\ta.....create presignature data (this could be done multiple times)"
        << "\n\tt.....create a signature within a group"
        << "\n\tq.....quit the player"
        ;
    return os.str(); 
}
    
void wait(int seconds)
{
  boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void threadListener(const int& port)
{
    try{
        std::cout << "Opening port on " << port << std::endl;
        boost::asio::io_context io_context;
        server s(io_context, port);
        io_context.run();
    }
    catch(boost::thread_interrupted&){
        std::cout << "listener thread interrupted : " << std::endl;
    }
    return;
}

int main(int argc, char* argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    try{
        if (argc != 4){
          std::cerr << "Usage: tsclient <ipaddress> <port> <name>\n";
          return 1;
        }



        
        player p(argv[3],std::string(), argv[1], argv[2]);
        
        SinglePlayer::Instance()->publicInfo() = p;
        
        std::cout << SinglePlayer::Instance()->publicInfo() << std::endl;        

        int portNo = std::atoi(argv[2]);
        boost::thread t{threadListener, portNo};
        
        std::string address ("127.0.0.1");
        std::string port ("8133");
        boost::asio::streambuf b;
        std::ostream os(&b);
        unsigned char userChoice('0');
        int n(-1);
        int m(-1);
        std::vector<std::string> ids;
        std::string grpid; 
        char message[1024];
        std::string themessage;
        int ekeyindex(0);
        int keycount(1);
        while (userChoice != 'q'){
            std::cin >> userChoice;
            switch(userChoice)
            {
                case '1':
                    createTimeRequest(SinglePlayer::Instance()->publicInfo() ,os); 
                    genericRequestResponse(os,b,address, port);
                    break;
                case 'r':
                    createRegisterPlayerRequest(SinglePlayer::Instance()->publicInfo() ,os);
                    genericRequestResponse(os,b,address, port);
                    break;
                case 'l':
                    createListPlayersRequest(SinglePlayer::Instance()->publicInfo() ,os);
                    genericRequestResponse(os,b,address,port);
                    break;
                case 'c':
                    std::cout << "Please input threshold size" << std::endl;
                    std::cin >> n;
                    std::cout << "please input group size" << std::endl;
                    std::cin >> m;
                    createThresholdGroupRequest(SinglePlayer::Instance()->publicInfo() ,n,m,os);
                    genericRequestResponse(os,b,address,port);
                    break;
                case 'p':
                    std::cout << "List the groups for the local player " 
                        <<  SinglePlayer::Instance()->publicInfo() << std::endl;
                    ids = SinglePlayer::Instance()->GroupIDS();
                    
                    for(std::vector<std::string>::const_iterator iter = ids.begin();
                        iter != ids.end();
                        ++iter)
                    {
                        std::cout << "Grp ID : " << *iter << std::endl;
                    }
                    break;
                case 'g':
                    std::cout << "list a single group details for a local player\nPlease supply a grp id" << std::endl;
                    std::cin >> grpid; 
                    try{
                        std::cout << SinglePlayer::Instance()->groupdetails(grpid) << std::endl;
                    }
                    catch(std::exception& e){
                        std::cout << e.what() << std::endl;
                    }
                    break; 
                case 'd':
                    std::cout << "Delete a group\nPlease enter a group id" << std::endl;
                    std::cin >> grpid;
                    createDeleteTSGroupRequest(SinglePlayer::Instance()->publicInfo(),grpid,os);
                    genericRequestResponse(os,b,address,port);
                    break;
                case 's':
                    std::cout << "share a secret within a group\nPlease supply a group id" << std::endl;
                    std::cin >> grpid;
                    createSecretSharingRequest (SinglePlayer::Instance()->publicInfo(),grpid, std::string("PRIVATEKEYSHARE"),os);
                    genericRequestResponse(os,b,address,port);
                    break;
                case 'a':
                    std::cout << "share presignature data within a group\nPlease supply a group id" << std::endl;
                    std::cin >> grpid;
                    std::cout << "please enter the number of keys to generate per player" <<std::endl;
                    std::cin >> keycount;
                    for(int i=0;i<keycount;++i){
                        createSecretSharingRequest(p,grpid, std::string("LITTLEK"),os);
                        genericRequestResponse(os,b,address,port);
                        createSecretSharingRequest(p,grpid, std::string("ALPHA"),os);
                        genericRequestResponse(os,b,address,port);
                        createEphemeralKeyDataRequest(p.userID(), grpid,os);
                        genericRequestResponse(os,b,address,port);
                    }
                    break;
                case 't':
                    std::cout << "Sign a message with a signature group\nPlease supply a group id" << std::endl;
                    std::cin >> grpid;
                    themessage = "this is a message to sign"; 
                    std::cout << "Please enter the index of the emphemeral key" << std::endl;
                    std::cin >> ekeyindex;
                    //std::getline(std::cin, themessage);
                    //std::cin.getline(message, 4096);
                    //themessage = message;
                    createSignatureRequest(p.userID(), grpid, themessage,ekeyindex ,os);    
                    genericRequestResponse(os,b,address,port);
                    break;         
                case 'h':
                    std::cout << Options () << std::endl;
                    break; 
                case 'q':
                    std::cout << "quit this badboy" << std::endl;
                    break;
                default:
                    std::cout << Options () << std::endl;
                    break;
            }
            // clear the streambuffers.
            os.flush(); 
            os.clear(); 
        }
    }
    catch (std::exception& e){
        std::cerr << "Exception: " << e.what() << "\n";
    }
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}

