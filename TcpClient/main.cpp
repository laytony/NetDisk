#include "tcpclient.h"

#include <QApplication>
#include "operatewidget.h"
#include "sharefile.h"
#include "files.h"

#include "chessboard.h"
#include <iostream>
#include "chessboard.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpClient::getInstance().show();
//    Files w;
//    w.show();
    //定义四种类
    //Chessboard chessboard(13, 40, 39, 67.5);
//    Chessboard chessboard;//棋盘大小 横向边界 众向边界 棋子大小
////    chessboard.i_Chessboard(19, 51, 51, 43.1);
//    chessboard.i_Chessboard(13, 40, 39, 67.5, 1);
//    chessboard.show();
    return a.exec();
}

//#include <iostream>
//#include "user.h"
//#include "ai.h"
//#include "chessgame.h"
//#include "chessboard.h"


//using namespace std;

//void PRINT(vector<vector<float>> &a)
//{
//    for (auto x : a) {
//        for (auto y : x) {
//            cout << y << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;
//}

//int main()
//{
//    User user;
//    AI ai;
//    //Chessboard chessboard(13, 40, 39, 67.5);
//    Chessboard chessboard;//棋盘大小 横向边界 众向边界 棋子大小
//    chessboard.i_Chessboard(19, 51, 51, 43.1);
//    chessboard.show();
//    Chessgame game(&user, &ai, &chessboard);//初始化游戏控制类用上述对象初始化
//    ///游戏开始
//    game.play(19);
//    return 0;
//}
