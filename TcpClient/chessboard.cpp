#include "chessboard.h"
#include "ui_chessboard.h"
#include <QIcon>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMouseEvent>
#include <QSound>
#include <QTimer>
#include <QtAlgorithms>
#include <QtGlobal>
#include <QtMath>
#include <windows.h>
#include <QDebug>
#include <iostream>
#include <QMessageBox>


using namespace  std;

Chessboard::Chessboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chessboard)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    this->setWindowIcon(QIcon("./res/wzq.ico"));
    this->setWindowTitle("五子棋");
    blackChess_img = QImage("./res/black.png");
    whiteChess_img = QImage("./res/white.png");
    mouse_x_pose = -1;
    mouse_y_pose = -1;
    ////
    ////
    chess_label[2] = ui->label_2;
    chess_label[3] = ui->label_3;
    chess_label[4] = ui->label_4;
    chess_label[5] = ui->label_5;
    chess_label[6] = ui->label_6;
    chess_label[7] = ui->label_7;
    chess_label[8] = ui->label_8;
    chess_label[9] = ui->label_9;
    chess_label[10] = ui->label_10;
    chess_label[11] = ui->label_11;
    chess_label[12] = ui->label_12;
    chess_label[13] = ui->label_13;
    chess_label[14] = ui->label_14;
    chess_label[15] = ui->label_15;
    chess_label[16] = ui->label_16;
    chess_label[17] = ui->label_17;
    chess_label[18] = ui->label_18;
    chess_label[19] = ui->label_19;
    chess_label[20] = ui->label_20;
    chess_label[21] = ui->label_21;
    chess_label[22] = ui->label_22;
    chess_label[23] = ui->label_23;
    chess_label[24] = ui->label_24;
    chess_label[25] = ui->label_25;
    chess_label[26] = ui->label_26;
    chess_label[27] = ui->label_27;
    chess_label[28] = ui->label_28;
    chess_label[29] = ui->label_29;
    chess_label[30] = ui->label_30;
    chess_label[31] = ui->label_31;
    chess_label[32] = ui->label_32;
    chess_label[33] = ui->label_33;
    chess_label[34] = ui->label_34;
    chess_label[35] = ui->label_35;
    chess_label[36] = ui->label_36;
    chess_label[37] = ui->label_37;
    chess_label[38] = ui->label_38;
    chess_label[39] = ui->label_39;
    chess_label[40] = ui->label_40;
    chess_label[41] = ui->label_41;
    chess_label[42] = ui->label_42;
    chess_label[43] = ui->label_43;
    chess_label[44] = ui->label_44;
    chess_label[45] = ui->label_45;
    chess_label[46] = ui->label_46;
    chess_label[47] = ui->label_47;
    chess_label[48] = ui->label_48;
    chess_label[49] = ui->label_49;
    chess_label[50] = ui->label_50;
    chess_label[51] = ui->label_51;
    chess_label[52] = ui->label_52;
    chess_label[53] = ui->label_53;
    chess_label[54] = ui->label_54;
    chess_label[55] = ui->label_55;
    chess_label[56] = ui->label_56;
    chess_label[57] = ui->label_57;
    chess_label[58] = ui->label_58;
    chess_label[59] = ui->label_59;
    chess_label[60] = ui->label_60;
    chess_label[61] = ui->label_61;
    chess_label[62] = ui->label_62;
    chess_label[63] = ui->label_63;
    chess_label[64] = ui->label_64;
    chess_label[65] = ui->label_65;
    chess_label[66] = ui->label_66;
    chess_label[67] = ui->label_67;
    chess_label[68] = ui->label_68;
    chess_label[69] = ui->label_69;
    chess_label[70] = ui->label_70;
    chess_label[71] = ui->label_71;
    chess_label[72] = ui->label_72;
    chess_label[73] = ui->label_73;
    chess_label[74] = ui->label_74;
    chess_label[75] = ui->label_75;
    chess_label[76] = ui->label_76;
    chess_label[77] = ui->label_77;
    chess_label[78] = ui->label_78;
    chess_label[79] = ui->label_79;
    chess_label[80] = ui->label_80;
    chess_label[81] = ui->label_81;
    chess_label[82] = ui->label_82;
    chess_label[83] = ui->label_83;
    chess_label[84] = ui->label_84;
    chess_label[85] = ui->label_85;
    chess_label[86] = ui->label_86;
    chess_label[87] = ui->label_87;
    chess_label[88] = ui->label_88;
    chess_label[89] = ui->label_89;
    chess_label[90] = ui->label_90;
    chess_label[91] = ui->label_91;
    chess_label[92] = ui->label_92;
    chess_label[93] = ui->label_93;
    chess_label[94] = ui->label_94;
    chess_label[95] = ui->label_95;
    chess_label[96] = ui->label_96;
    chess_label[97] = ui->label_97;
    chess_label[98] = ui->label_98;
    chess_label[99] = ui->label_99;
    chess_label[100] = ui->label_100;
    chess_label[101] = ui->label_101;
    chess_label[102] = ui->label_102;
    chess_label[103] = ui->label_103;
    chess_label[104] = ui->label_104;
    chess_label[105] = ui->label_105;
    chess_label[106] = ui->label_106;
    chess_label[107] = ui->label_107;
    chess_label[108] = ui->label_108;
    chess_label[109] = ui->label_109;
    chess_label[110] = ui->label_110;
    chess_label[111] = ui->label_111;
    chess_label[112] = ui->label_112;
    chess_label[113] = ui->label_113;
    chess_label[114] = ui->label_114;
    chess_label[115] = ui->label_115;
    chess_label[116] = ui->label_116;
    chess_label[117] = ui->label_117;
    chess_label[118] = ui->label_118;
    chess_label[119] = ui->label_119;
    chess_label[120] = ui->label_120;
    chess_label[121] = ui->label_121;
    chess_label[122] = ui->label_122;
    chess_label[123] = ui->label_123;
    chess_label[124] = ui->label_124;
    chess_label[125] = ui->label_125;
    chess_label[126] = ui->label_126;
    chess_label[127] = ui->label_127;
    chess_label[128] = ui->label_128;
    chess_label[129] = ui->label_129;
    chess_label[130] = ui->label_130;
    chess_label[131] = ui->label_131;
    chess_label[132] = ui->label_132;
    chess_label[133] = ui->label_133;
    chess_label[134] = ui->label_134;
    chess_label[135] = ui->label_135;
    chess_label[136] = ui->label_136;
    chess_label[137] = ui->label_137;
    chess_label[138] = ui->label_138;
    chess_label[139] = ui->label_139;
    chess_label[140] = ui->label_140;
    chess_label[141] = ui->label_141;
    chess_label[142] = ui->label_142;
    chess_label[143] = ui->label_143;
    chess_label[144] = ui->label_144;
    chess_label[145] = ui->label_145;
    chess_label[146] = ui->label_146;
    chess_label[147] = ui->label_147;
    chess_label[148] = ui->label_148;
    chess_label[149] = ui->label_149;
    chess_label[150] = ui->label_150;
    chess_label[151] = ui->label_151;
    chess_label[152] = ui->label_152;
    chess_label[153] = ui->label_153;
    chess_label[154] = ui->label_154;
    chess_label[155] = ui->label_155;
    chess_label[156] = ui->label_156;
    chess_label[157] = ui->label_157;
    chess_label[158] = ui->label_158;
    chess_label[159] = ui->label_159;
    chess_label[160] = ui->label_160;
    chess_label[161] = ui->label_161;
    chess_label[162] = ui->label_162;
    chess_label[163] = ui->label_163;
    chess_label[164] = ui->label_164;
    chess_label[165] = ui->label_165;
    chess_label[166] = ui->label_166;
    chess_label[167] = ui->label_167;
    chess_label[168] = ui->label_168;
    chess_label[169] = ui->label_169;
    chess_label[170] = ui->label_170;
    chess_label[171] = ui->label_171;
    chess_label[172] = ui->label_172;
    chess_label[173] = ui->label_173;
    chess_label[174] = ui->label_174;
    chess_label[175] = ui->label_175;
    chess_label[176] = ui->label_176;
    chess_label[177] = ui->label_177;
    chess_label[178] = ui->label_178;
    chess_label[179] = ui->label_179;
    chess_label[180] = ui->label_180;
    ////
    ///
    ///
    ///
//    inint_scoremap();
    player = new QMediaPlayer(this);
    plist = new QMediaPlaylist(player);
    ai_timer = new QTimer;
    end_timer = new QTimer;
    connect(end_timer, &QTimer::timeout, this, &Chessboard::isOver);
    Init(19);
    connect(ai_timer, &QTimer::timeout, this, &Chessboard::AI_opration);
//    connect(this, &Chessboard::ai_go_signal, this, &Chessboard::AI_opration);
//    connect()
}

Chessboard::~Chessboard()
{
    player->stop();
    player->stateChanged(QMediaPlayer::StoppedState);//设置停止播放
    delete ui;
}

void Chessboard::closeEvent(QCloseEvent *event)
{
    if(this->isVisible()) {
        this->hide();
        player->stop();
    } else {
        QMessageBox::information(this, "Notice", "You can't close the window");
    }
    // event->ignore();
}



void Chessboard:: mousePressEvent(QMouseEvent *event)
{
    //单击
    // 如果是鼠标左键按下
    if (event->button() == Qt::LeftButton) {
        qDebug() << "left click";
        mouse_x_pose = event->x();
        mouse_y_pose = event->y();
        ChessPos pos;
        if(is_validchessdown(mouse_x_pose, mouse_y_pose, &pos)) {
            if(FLAG) {
                chessDown(&pos, CHESS_WHITE);
            } else {
                chessDown(&pos, CHESS_BLACK);
            }
            emit ai_go_signal();
            ai_timer->start(500);
            //直到出现有效点击 跳出循环 进行进行下子函数，当出现有效点击伴随音效
        }
    }
    // 如果是鼠标右键按下
//    else if (event->button() == Qt::RightButton) {
//        //qDebug() << "right click";
//        setMouseState(MouseState::R_C, 0);
//    } else if (event->button() == Qt::MidButton) {
//        //qDebug() << "mid click";
//        setMouseState(MouseState::M_C, 0);
    //    }
}

bool Chessboard::eventFilter(QObject *object, QEvent *event)//回车重新开始
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = dynamic_cast <QKeyEvent *> (event);
        if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) { //步骤三
            i_Chessboard(13, 40, 39, 67.5, 1);
            return true;
        }
    }
}

void Chessboard::AI_opration()//AI操作
{
    ai_timer->stop();
    qDebug() << "ai操作";
    ChessPos pos = AIthink();
    qDebug() << "AI_x" << pos.col;
    qDebug() << "AI_y" << pos.row;
    Sleep(300); //AI 假装思考,获取下子位置
    if(FLAG) {
        chessDown(&pos, CHESS_BLACK);
    } else {
        chessDown(&pos, CHESS_WHITE);
    }
}


void Chessboard::inint_scoremap()
{
    float score{ 0 };
    int n = boardSize;
    int left{ 0 }, right{ n - 1 }, top{ 0 }, down{ n - 1 };//四方边界
    int index = n / 2 + 1;//模拟的圈数
    while (n--) {
        for (int i{ left }; i <= right && !getChessData(top, i); ++i) { //从左至右
            scoreMap[top][i] = score;
        }
        top++;
        for (int i{ top }; i <= down && !getChessData(i, right); ++i) { //从上到下
            scoreMap[i][right] = score;
        }
        right--;
        for (int i{ right }; i >= left && !getChessData(down, i); --i) {//从右到左
            scoreMap[down][i] = score;
        }
        down--;
        for (int i{ down }; i >= top && !getChessData(i, left); --i) { //从下到上
            scoreMap[i][left] = score;
        }
        left++;
        score += 0.4;//每一圈增加的分值
    }
}

void Chessboard::AIgetscore()
{
    //1. 没有对跳三和跳四进行判断。实际上，跳三和跳四的价值与连三连四的价值，是完全相同的。而现在的AI只计算了连三和连四，没有考虑跳三跳四，所以就会错失“好棋”！
    //. 没有对黑棋设置“禁手”。因为五子棋已经发展到“黑棋先行必胜”的套路，所以职业五子棋比赛，会对黑棋设置以下“禁手”。
    //三三禁手
    //四四禁手
    //长连禁手
    int personNum = 0; //棋手方（黑棋）多少个连续的棋子
    int aiNum = 0; //AI方连续有多少个连续的棋子
    int emptyNum = 0; // 该方向上空白位的个数
    // 评分向量数组清零
    for (int i = 0; i < scoreMap.size(); i++) {
        for (int j = 0; j < scoreMap[i].size(); j++) {
            scoreMap[i][j] = 0;
        }
    }
    int size = boardSize;
    //初始化评分矩阵 根据位置制定初始评分 越靠近中间评分越高
    //inint_scoremap();
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            //对每个点进行计算
            if (getChessData(row, col)) {//存在数据跳过
                continue;
            }
            for (int y = -1; y <= 0; y++) {        //Y的范围还是-1， 0
                for (int x = -1; x <= 1; x++) {    //X的范围是 -1,0,1
                    if (y == 0 && x == 0) {
                        continue;
                    }
                    if (y == 0 && x != 1) {
                        continue;    //当y=0时，仅允许x=1
                    }
                    personNum = 0;
                    aiNum = 0;
                    emptyNum = 0;
                    // 假设黑棋在该位置落子，会构成什么棋型
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row + i * y;
                        int curCol = col + i * x;
                        if (curRow >= 0 && curRow < size &&
                                curCol >= 0 && curCol < size &&
                                getChessData(curRow, curCol) == 1) {
                            personNum++;
                        } else if (curRow >= 0 && curRow < size &&
                                   curCol >= 0 && curCol < size &&
                                   getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        } else {
                            break;
                        }
                    }
                    // 反向继续计算
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;
                        if (curRow >= 0 && curRow < size &&
                                curCol >= 0 && curCol < size &&
                                getChessData(curRow, curCol) == 1) {
                            personNum++;
                        } else if (curRow >= 0 && curRow < size &&
                                   curCol >= 0 && curCol < size &&
                                   getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        } else {
                            break;
                        }
                    }
                    if (personNum == 1) { //连2
                        scoreMap[row][col] += 10;
                    } else if (personNum == 2) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 30;
                        } else if (emptyNum == 2) {
                            scoreMap[row][col] += 40;
                        }
                    } else if (personNum == 3) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] = 60;
                        } else if (emptyNum == 2) {
                            scoreMap[row][col] = 5000; //200，
                        }
                    } else if (personNum == 4) {
                        scoreMap[row][col] = 20000;
                    }
                    // 假设白棋在该位置落子，会构成什么棋型
                    emptyNum = 0;
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row + i * y;
                        int curCol = col + i * x;
                        if (curRow >= 0 && curRow < size &&
                                curCol >= 0 && curCol < size &&
                                getChessData(curRow, curCol) == -1) {
                            aiNum++;
                        } else if (curRow >= 0 && curRow < size &&
                                   curCol >= 0 && curCol < size &&
                                   getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        } else {
                            break;
                        }
                    }
                    for (int i = 1; i <= 4; i++) {
                        int curRow = row - i * y;
                        int curCol = col - i * x;
                        if (curRow >= 0 && curRow < size &&
                                curCol >= 0 && curCol < size &&
                                getChessData(curRow, curCol) == -1) {
                            aiNum++;
                        } else if (curRow >= 0 && curRow < size &&
                                   curCol >= 0 && curCol < size &&
                                   getChessData(curRow, curCol) == 0) {
                            emptyNum++;
                            break;
                        } else {
                            break;
                        }
                    }
                    if (aiNum == 0) {
                        scoreMap[row][col] += 5;
                    } else if (aiNum == 1) {
                        scoreMap[row][col] += 10;
                    } else if (aiNum == 2) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 25;
                        } else if (emptyNum == 2) {
                            scoreMap[row][col] += 50;
                        }
                    } else if (aiNum == 3) {
                        if (emptyNum == 1) {
                            scoreMap[row][col] += 55;
                        } else if (emptyNum == 2) {
                            scoreMap[row][col] += 10000;
                        }
                    } else if (aiNum >= 4) {
                        scoreMap[row][col] += 30000;
                    }
                }
            }
        }
    }
    /*
    constexpr static int dir[8][2]{ {-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1} };//八个位置
    int usernum{ 0 };//计算下完当前位置时连续黑子的个数
    int ainum{ 0 };//计算下完当前位置时连续白子的个数

    //初始化权值矩阵
    int n = chessboard->getBoardsize();//获取棋局大小
    inint_scoremap(scoreMap, n);
    for (auto x : scoreMap) {
        for (auto y : x) {
            std::cout << y << "   ";
        }
        cout << endl;
    }
    //获取情况种类
    /// 得分    敌方     我方     情况
    ///         10       10      连2
    ///         30       25      死连3
    ///         50       55      活连3
    ///         70       65      死连4
    ///         300      10000   活连4
    ///         20000    30000   连5

    constexpr static int scoretype[6][2]{ {10,10},{30,25},{50,55},{70,65},{300,10000},{20000,30000} };//敌方为0 我方为1

    /// 获取对方当前位置得分：
    auto get_otherscore = [&](int i, int j, float curscore)->int {
        for (auto d : dir) {//八个位置
            int blacksum{ 1 };//连续黑子的个数
            int x = d[0] + i;
            int y = d[1] + j;
            //黑子连续个数
            while (0 <= x && x < n && 0 <= y && y < n && chessboard->getChessData(x, y) == 1) {//连续1 即黑子的个数
                blacksum++;
                x += d[0];
                y += d[1];
            }
            if (blacksum > 1) {
                if (0 <= x && x < n && 0 <= y && y < n && chessboard->getChessData(x, y) == 0) {//活连
                    switch (blacksum) {
                    case(2):curscore += scoretype[0][0];
                        break;
                    case(3):curscore += scoretype[2][0];
                        break;
                    case(4):curscore += scoretype[4][0];
                        break;
                    default:curscore += scoretype[5][0];
                        break;
                    }
                }
                else {//死连，要么末尾为白子 要么越界
                    switch (blacksum) {
                    case(2):curscore += scoretype[0][0];
                        break;
                    case(3):curscore += scoretype[1][0];
                        break;
                    case(4):curscore += scoretype[3][0];;
                        break;
                    default:curscore += scoretype[5][0];;
                        break;
                    }
                }
            }
        }
        return curscore;
    };

    /// 获取自己当前位置得分：

    auto get_selfscore = [&](int i, int j, float curscore)->int {
        for (auto d : dir) {//八个位置
            int whitesum{ 0 };//连续白子的个数
            int x = d[0] + i;
            int y = d[1] + j;
            //黑子连续个数
            while (0 <= x && x < n && 0 <= y && y < n && chessboard->getChessData(x, y) == -1) {//连续-1 即白子的个数
                whitesum++;
                x += d[0];
                y += d[1];
            }
            if (whitesum > 0) {
                if (0 <= x && x < n && 0 <= y && y < n && chessboard->getChessData(x, y) == 0) {//活连
                    switch (whitesum) {
                    case(2):curscore += scoretype[0][1];
                        break;
                    case(3):curscore += scoretype[2][1];
                        break;
                    case(4):curscore += scoretype[4][1];
                        break;
                    default:curscore += scoretype[5][1];
                        break;
                    }
                }
                else {//死连，要么末尾为黑子子 要么越界
                    switch (whitesum) {
                    case(2):curscore += scoretype[0][1];
                        break;
                    case(3):curscore += scoretype[1][1];
                        break;
                    case(4):curscore += scoretype[3][1];
                        break;
                    default:curscore += scoretype[5][1];
                        break;
                    }
                }
            }
        }
        return curscore;
        //分别计算当前位置为白子 或者 黑子时的得分
    for (int r{ 0 }; r != n; ++r) {
        for (int c{ 0 }; c != n; ++c) {
            if (chessboard->getChessData(r, c) == 0) {//当前位置为空，未下子，才能计算得分
                //更新当前位置得分
                float curscore = scoreMap[r][c];
                scoreMap[r][c] = curscore + get_otherscore(r, c, curscore) + get_selfscore(r, c, curscore);
            }
        }
    }
    */
}

bool Chessboard::is_validpos(int r, int c)
{
    return 0 <= r && r < boardSize && 0 <= c && c < boardSize;
}

void Chessboard::AIgetscore_ray()
{
    for (auto &x : scoreMap) {//清空权值矩阵
        for (auto &y : x) {
            y = 0;
        }
    }
    //初始化权值矩阵
    inint_scoremap();
    //定义四个主方向
    constexpr static int dir[4][2] { {0, 1}, {1, -1}, {1, 1}, {1, 0} }; //设置四个主方位 右 右下 下 左下
    //状态向量
    vector<int>state(9, 2);//初始化为2 若赋值之后存在2表明此处越界了！！！
    //统计当前方向上正负4个位置上的棋子状态0 1 2 3   （当前要下的子）   5  6  7  8
    int n = boardSize;
    for (int r{ 0 }; r != n; ++r) {
        for (int c{ 0 }; c != n; ++c) {
            if (chessMap[r][c]) {
                continue;//当前位置有子直接跳过
            }
            //分四个方向计算状态向量
            for (auto d : dir) {
                int k = 4;//当前state周围半径
                while (k) {//一侧
                    int x = r + k * d[0];
                    int y = c + k * d[1];
                    if (is_validpos(x, y)) {//位置合法
                        //填充8 7 6 5 四个下标
                        state[4 + k] = chessMap[x][y];//填写state的前四个位置 第五个位置为0 即为待下的位置
                    }
                    k--;
                }
                k = 4;
                while (k) {//另一侧
                    int x = r - k * d[0];
                    int y = c - k * d[1];
                    if (is_validpos(x, y)) {//位置合法
                        //填写0 1 2 3四个位置
                        state[4 - k] = chessMap[x][y];//填写state的前四个位置 第五个位置为0 即为待下的位置
                    }
                    k--;
                }//至此已经完整得到了一个点的 单侧方向上的状态向量
                //下面进行分析状态向量、单个为下棋的空位 有四个方向的状态向量一共分析四个方向 累加四个状态
                scoreMap[r][c] += getsinglescore(state);//累加当前一个状态
                //状态向量重新赋值
                state = vector<int>(9, 2);
            }
        }
    }
}

ChessPos Chessboard::AIthink()
{
    srand((unsigned)time(NULL));//随机数种子
    //更新权值矩阵
    //AIgetscore();
    AIgetscore_ray();
    /*
    cout <<  endl;
    for (auto x : scoreMap) {
        for (auto y : x) {
            cout << y << "   ";
        }
        cout << endl;
    }
    */
    float MAXSCORE{ 0 };
    vector<ChessPos>probpos;//所有可能的位置 可能有多个位置
    int row{ 0 }, col{ 0 };//走棋的位置
    int n = boardSize;//获取棋局大小
    //首先获取走棋的位置
    for (int r{ 0 }; r != n; ++r) {
        for (int c{ 0 }; c != n; ++c) {
            if (getChessData(r, c) == 0) {//当前位置为空 未下棋 才计算分数
                if (scoreMap[r][c] > MAXSCORE) {
                    MAXSCORE = scoreMap[r][c];
                    probpos.clear();
                    probpos.push_back({ r, c });
                } else if (scoreMap[r][c] == MAXSCORE) {
                    probpos.push_back({ r, c });
                }
            }
        }
    }
    //cout << probpos.size() << endl;
    int index = rand() % probpos.size();//随机获取落子坐标 有多个
    //cout << index << endl;//要取得 [a,b] 的随机整数，使用 (rand () % (b-a+1))+ a;
    return probpos[index];
}

float Chessboard::getsinglescore(vector<int> &state)
{
    float score{ 0 };
    //确保对黑白棋的棋形态只考虑最有价值的一种情况
    //各种情况的评分从连2 ->连5 0 ->AI方 进攻型黑方
    constexpr static int SCORELIST[2][6] { { 10, 25, 50, 70, 300, 20000 }, { 10, 30, 55, 80, 10000, 30000 }};
    //考虑当前下子对我方有利得分 AI即当前下子对白方的有利情况
    do {
        //优先级从高往低排列 比如当出现连3时包括了连2  就不考虑连2 了
        //1 白连五 5 种情况 其他0位置不管都+ 30000 //当前AI方下此处就形成了连5
        if ((state[0] == -1 && state[1] == -1 && state[2] == -1 && state[3] == -1) ||
                (state[1] == -1 && state[2] == -1 && state[3] == -1 && state[5] == -1) ||
                (state[2] == -1 && state[3] == -1 && state[5] == -1 && state[6] == -1) ||
                (state[3] == -1 && state[5] == -1 && state[6] == -1 && state[7] == -1) ||
                (state[5] == -1 && state[6] == -1 && state[7] == -1 && state[8] == -1)) {
            score += SCORELIST[0][5];
            break;//最多只考虑单个方向上的一种情况
        }
        //2 白活四 + 10000 4种情况
        if ((state[1] == -1 && state[2] == -1 && state[3] == -1 && state[0] == 0 && state[5] == 0) ||
                (state[2] == -1 && state[3] == -1 && state[5] == -1 && state[1] == 0 && state[6] == 0) ||
                (state[3] == -1 && state[5] == -1 && state[6] == -1 && state[2] == 0 && state[7] == 0) ||
                (state[5] == -1 && state[6] == -1 && state[7] == -1 && state[3] == 0 && state[8] == 0)) {
            score += SCORELIST[0][4];
            break;
        }
        //3 白死四 + 65    8 + 8种情况
        if ((state[1] == -1 && state[2] == -1 && state[3] == -1 && state[0] == 1 && state[5] == 0) ||
                (state[1] == -1 && state[2] == -1 && state[3] == -1 && state[0] == 0 && state[5] == 1) ||
                (state[1] == -1 && state[2] == -1 && state[3] == -1 && state[0] == 2 && state[5] == 0) ||
                (state[1] == -1 && state[2] == -1 && state[3] == -1 && state[0] == 0 && state[5] == 2) ||
                (state[2] == -1 && state[3] == -1 && state[5] == -1 && state[1] == 1 && state[6] == 0) ||
                (state[2] == -1 && state[3] == -1 && state[5] == -1 && state[1] == 0 && state[6] == 1) ||
                (state[2] == -1 && state[3] == -1 && state[5] == -1 && state[1] == 2 && state[6] == 0) ||
                (state[2] == -1 && state[3] == -1 && state[5] == -1 && state[1] == 0 && state[6] == 2) ||
                (state[3] == -1 && state[5] == -1 && state[6] == -1 && state[2] == 1 && state[7] == 0) ||
                (state[3] == -1 && state[5] == -1 && state[6] == -1 && state[2] == 0 && state[7] == 1) ||
                (state[3] == -1 && state[5] == -1 && state[6] == -1 && state[2] == 2 && state[7] == 0) ||
                (state[3] == -1 && state[5] == -1 && state[6] == -1 && state[2] == 0 && state[7] == 2) ||
                (state[5] == -1 && state[6] == -1 && state[7] == -1 && state[3] == 1 && state[8] == 0) ||
                (state[5] == -1 && state[6] == -1 && state[7] == -1 && state[3] == 0 && state[8] == 1) ||
                (state[5] == -1 && state[6] == -1 && state[7] == -1 && state[3] == 2 && state[8] == 0) ||
                (state[5] == -1 && state[6] == -1 && state[7] == -1 && state[3] == 0 && state[8] == 2)) {
            score += SCORELIST[0][3];
            break;
        }
        //4 白活三 + 55   3种情况
        if ((state[2] == -1 && state[3] == -1 && state[1] == 0 && state[5] == 0) ||
                (state[3] == -1 && state[5] == -1 && state[2] == 0 && state[6] == 0) ||
                (state[5] == -1 && state[6] == -1 && state[3] == 0 && state[7] == 0)) {
            score += SCORELIST[0][2];
            break;
        }
        //5 白死三+ 25 6 + 6种情况
        if ((state[2] == -1 && state[3] == -1 && state[1] == 1 && state[5] == 0) ||
                (state[2] == -1 && state[3] == -1 && state[1] == 0 && state[5] == 1) ||
                (state[2] == -1 && state[3] == -1 && state[1] == 2 && state[5] == 0) ||
                (state[2] == -1 && state[3] == -1 && state[1] == 0 && state[5] == 2) ||
                (state[3] == -1 && state[5] == -1 && state[2] == 1 && state[6] == 0) ||
                (state[3] == -1 && state[5] == -1 && state[2] == 0 && state[6] == 1) ||
                (state[3] == -1 && state[5] == -1 && state[2] == 2 && state[6] == 0) ||
                (state[3] == -1 && state[5] == -1 && state[2] == 0 && state[6] == 2) ||
                (state[5] == -1 && state[6] == -1 && state[3] == 1 && state[7] == 0) ||
                (state[5] == -1 && state[6] == -1 && state[3] == 0 && state[7] == 1) ||
                (state[5] == -1 && state[6] == -1 && state[3] == 2 && state[7] == 0) ||
                (state[5] == -1 && state[6] == -1 && state[3] == 0 && state[7] == 2)) {
            score += SCORELIST[0][1];
            break;
        }
        //6 白连2 +10 2种情况不考虑周围
        if (state[3] == -1 || state[5] == -1) {
            score += SCORELIST[0][0];
            break;
        }
    } while (0);
    //考虑当前下子对敌方有利得分 即当前玩家若下此处 state[4] 对玩家 黑棋 的有利情况
    do {
        //优先级从高往低排列 比如当出现连3时包括了连2  就不考虑连2 了
        //1 黑连五 5 种情况 其他0位置不管都+ 20000 //当前对方下此处就形成了连5
        if ((state[0] == 1 && state[1] == 1 && state[2] == 1 && state[3] == 1) ||
                (state[1] == 1 && state[2] == 1 && state[3] == 1 && state[5] == 1) ||
                (state[2] == 1 && state[3] == 1 && state[5] == 1 && state[6] == 1) ||
                (state[3] == 1 && state[5] == 1 && state[6] == 1 && state[7] == 1) ||
                (state[5] == 1 && state[6] == 1 && state[7] == 1 && state[8] == 1)) {
            score += SCORELIST[1][5];
            break;//最多只考虑单个方向上的一种情况
        }
        //2 黑活四 + 300 4种情况  //周围不能越界
        if ((state[1] == 1 && state[2] == 1 && state[3] == 1 && state[0] == 0 && state[5] == 0) ||
                (state[2] == 1 && state[3] == 1 && state[5] == 1 && state[1] == 0 && state[6] == 0) ||
                (state[3] == 1 && state[5] == 1 && state[6] == 1 && state[2] == 0 && state[7] == 0) ||
                (state[5] == 1 && state[6] == 1 && state[7] == 1 && state[3] == 0 && state[8] == 0)) {
            score += SCORELIST[1][4];
            break;
        }
        //3 黑死四 + 70    8 + 8 种情况 存在越界即一边是2
        if ((state[1] == 1 && state[2] == 1 && state[3] == 1 && state[0] == -1 && state[5] == 0) ||
                (state[1] == 1 && state[2] == 1 && state[3] == 1 && state[0] == 0 && state[5] == -1) ||
                (state[1] == 1 && state[2] == 1 && state[3] == 1 && state[0] == 2 && state[5] == 0) ||
                (state[1] == 1 && state[2] == 1 && state[3] == 1 && state[0] == 0 && state[5] == 2) ||
                (state[2] == 1 && state[3] == 1 && state[5] == 1 && state[1] == -1 && state[6] == 0) ||
                (state[2] == 1 && state[3] == 1 && state[5] == 1 && state[1] == 0 && state[6] == -1) ||
                (state[2] == 1 && state[3] == 1 && state[5] == 1 && state[1] == 2 && state[6] == 0) ||
                (state[2] == 1 && state[3] == 1 && state[5] == 1 && state[1] == 0 && state[6] == 2) ||
                (state[3] == 1 && state[5] == 1 && state[6] == 1 && state[2] == -1 && state[7] == 0) ||
                (state[3] == 1 && state[5] == 1 && state[6] == 1 && state[2] == 0 && state[7] == -1) ||
                (state[3] == 1 && state[5] == 1 && state[6] == 1 && state[2] == 2 && state[7] == 0) ||
                (state[3] == 1 && state[5] == 1 && state[6] == 1 && state[2] == 0 && state[7] == 2) ||
                (state[5] == 1 && state[6] == 1 && state[7] == 1 && state[3] == -1 && state[8] == 0) ||
                (state[5] == 1 && state[6] == 1 && state[7] == 1 && state[3] == 0 && state[8] == -1) ||
                (state[5] == 1 && state[6] == 1 && state[7] == 1 && state[3] == 2 && state[8] == 0) ||
                (state[5] == 1 && state[6] == 1 && state[7] == 1 && state[3] == 0 && state[8] == 2)) {
            score += SCORELIST[1][3];
            break;
        }
        //4 黑活三 + 50   3种情况
        if ((state[2] == 1 && state[3] == 1 && state[1] == 0 && state[5] == 0) ||
                (state[3] == 1 && state[5] == 1 && state[2] == 0 && state[6] == 0) ||
                (state[5] == 1 && state[6] == 1 && state[3] == 0 && state[7] == 0)) {
            score += SCORELIST[1][2];
            break;
        }
        //5 黑死三+ 30 6 + 6 种情况 存在越界
        if ((state[2] == 1 && state[3] == 1 && state[1] == -1 && state[5] == 0) ||
                (state[2] == 1 && state[3] == 1 && state[1] == 0 && state[5] == -1) ||
                (state[2] == 1 && state[3] == 1 && state[1] == 2 && state[5] == 0) ||
                (state[2] == 1 && state[3] == 1 && state[1] == 0 && state[5] == 2) ||
                (state[3] == 1 && state[5] == 1 && state[2] == -1 && state[6] == 0) ||
                (state[3] == 1 && state[5] == 1 && state[2] == 0 && state[6] == -1) ||
                (state[3] == 1 && state[5] == 1 && state[2] == 2 && state[6] == 0) ||
                (state[3] == 1 && state[5] == 1 && state[2] == 0 && state[6] == 2) ||
                (state[5] == 1 && state[6] == 1 && state[3] == -1 && state[7] == 0) ||
                (state[5] == 1 && state[6] == 1 && state[3] == 0 && state[7] == -1) ||
                (state[5] == 1 && state[6] == 1 && state[3] == 2 && state[7] == 0) ||
                (state[5] == 1 && state[6] == 1 && state[3] == 0 && state[7] == 2)) {
            score += SCORELIST[1][1];
            break;
        }
        //黑连2 +10 2种情况不考虑周围
        if (state[3] == 1 || state[5] == 1) {
            score += SCORELIST[1][0];
            break;
        }
    } while (0);
    return score;
}




void Chessboard::updatechessmap(ChessPos *chesspos)
{
    lastPos = *chesspos;
    chessMap[chesspos->row][chesspos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;//playerflag为true时代表黑子 否则代表白子
    playerFlag = !playerFlag;//黑白方交换 下一次轮到白方不能使用 !=
}

void Chessboard::i_Chessboard(int boardSize, int margin_x, int margin_y, float chess_size, int flag)//0表示先手 1表示后手
{
    this->margin_x = margin_x;//this代指目前赋值
    this->margin_y = margin_y;
    this->boardSize = boardSize;
    this->chess_size = chess_size;
    chessMap = vector<vector<int>>(boardSize, vector<int>(boardSize, 0));//初始化权值矩阵
    QString path;
    scoreMap = vector<vector<float>>(boardSize, vector<float>(boardSize, 0));
    if(boardSize == 13) {
        path = "./res/BOARD.jpg";
//    } else if(size == 15) {
//        path = "./res/board19.png";
    } else if(boardSize == 19) {
        path = "./res/board19.png";
    }
    int offset{0};
    chess_img = QImage(path);
    ui->label->setGeometry(0, offset, chess_img.width(), chess_img.height());
    ui->label->setFixedSize(chess_img.width(), chess_img.height());
    this->setFixedSize(chess_img.width(), chess_img.height() + offset);
    ui->label->setPixmap(QPixmap(path));
//    QSound::play("./res/start.WAV");
//    player_1->setMedia(QUrl::fromLocalFile("./res/bg.mp3"));
//    QMediaPlaylist *plist = new QMediaPlaylist(player);
//    plist->addMedia(QUrl::fromLocalFile("./res/bg.mp3"));
//    plist->setPlaybackMode(QMediaPlaylist::Loop);
//    player->setPlaylist(plist);
//    player_1->play();//循环播放bg
//    Sleep(1000);
//    player_1->stop();
    //棋盘清理
    for (auto &x : chessMap) {
        for (auto &y : x) {
            y = 0;//清空为未下状态
        }
    }
    playerFlag = true;//定义黑棋开始走
    playerFlag = CHESS_BLACK;//目前下棋方默认为黑方 即为 1
    for(auto &x : chess_label) {
        x.second->hide();
    }
    FLAG = flag;
    if(FLAG == 1) {
        AI_opration();//ai先走一步
    }
}

void Chessboard::Init(int size)
{
    QString path;
    if(size == 13) {
        path = "./res/BOARD.jpg";
//    } else if(size == 15) {
//        path = "./res/board19.png";
    } else if(size == 19) {
        path = "./res/board19.png";
    }
    int offset{0};
    chess_img = QImage(path);
    ui->label->setGeometry(0, offset, chess_img.width(), chess_img.height());
    ui->label->setFixedSize(chess_img.width(), chess_img.height());
    this->setFixedSize(chess_img.width(), chess_img.height() + offset);
    ui->label->setPixmap(QPixmap(path));
    QSound::play("./res/start.WAV");
    plist->addMedia(QUrl::fromLocalFile("./res/bg.mp3"));
    plist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(plist);
    player->play();//循环播放bg
    //棋盘清理
    for (auto &x : chessMap) {
        for (auto &y : x) {
            y = 0;//清空为未下状态
        }
    }
    playerFlag = true;//定义黑棋开始走
}

int Chessboard::getBoardsize()
{
    return boardSize;
}

bool Chessboard::is_validchessdown(int x, int y, ChessPos *pos)
{
    //标志是否为有效落子
    bool flag{ false };
    //点击位置出现位置的前一线 向向下取整 ，row col 从0开始
    int col = (x - margin_x) / chess_size;
    int row = (y - margin_y) / chess_size;
    //最小无位置，点击位置所在 方框的左 和 上边界 最小 x y 距离
    int leftxd = col * chess_size + margin_x;
    int rightyd = row * chess_size + margin_y;
    //允许鼠标点击的最大模糊距离 最大偏差
    float offset = chess_size * 0.4;
    //当前位置距离方框四周的位置
    auto get_dis = [&](int x1, int y1, int x2, int y2)->float {
        return qSqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    };
    float dis{ 0 };//到四个点的距离
    do {
        //获取点击点到 左上方的距离，注意防止重复点击
        dis = get_dis(x, y, leftxd, rightyd);
        if (dis <= offset) {
            pos->row = row;
            pos->col = col;
            chessMap[pos->row][pos->col] == 0 ? flag = true : 1;
            break;
        }
        //到左下角的距离
        dis = get_dis(x, y, leftxd, rightyd + chess_size);
        if (dis <= offset) {
            pos->row = row + 1;
            pos->col = col;
            chessMap[pos->row][pos->col] == 0 ? flag = true : 1;
            break;
        }
        //到右下角的距离
        dis = get_dis(x, y, leftxd + chess_size, rightyd + chess_size);
        if (dis <= offset) {
            pos->row = row + 1;
            pos->col = col + 1;
            chessMap[pos->row][pos->col] == 0 ? flag = true : 1;
            break;
        }
        //到右上角的距离
        dis = get_dis(x, y, leftxd + chess_size, rightyd);
        if (dis <= offset) {
            pos->row = row;
            pos->col = col + 1;
            chessMap[pos->row][pos->col] == 0 ? flag = true : 1;
            break;
        }
    } while (0);//只进行一次 在外部循环User_opration中循环进行判断落子是否有效
    return flag;
}

void Chessboard::chessDown(ChessPos *pos, Chess_kind kind)
{
    QSound::play("./res/down7.WAV");//加载落子声音提示
    //计算加载落子位置的左上角 坐标（图片左上角坐标）
    int x = margin_x + (pos->col) * chess_size - 0.5 * chess_size;//x,y为左上角位置
    int y = margin_y + (pos->row) * chess_size - 0.5 * chess_size;
    int id = pos->col * boardSize + pos->row + 2;
//    qDebug() << "x:" << x;
//    qDebug() << "y:" << y;
//    qDebug() << "col:" << pos->col;
//    qDebug() << "row:" << pos->row;
//    qDebug() << "id:" << id;
    chess_label[id]->setGeometry(x, y, chess_size, chess_size);
    if (kind == CHESS_WHITE) {
        QPixmap pxm("./res/white.png");
        chess_label[id]->setPixmap(pxm);
    } else {
        QPixmap pxm("./res/black.png");
        chess_label[id]->setPixmap(pxm);
    }
    chess_label[id]->setScaledContents(true); //让图片填充满QLabel
    chess_label[id]->show();
    //下完棋子更新棋盘状态
    updatechessmap(pos);//更新棋盘状态
    end_timer->start(100);
//    if(isOver()) {
//        this->setMouseTracking(false);
//    }
}

int Chessboard::getChessData(ChessPos *pos)
{
    return chessMap[pos->row][pos->col];
}

int Chessboard::getChessData(int row, int col)
{
    return chessMap[row][col];
}

bool Chessboard::CheckWin()
{
    /*
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    int row = lastPos.row;
    int col = lastPos.col;

    for (int i = 0; i < 5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if (col - i >= 0 &&
            col - i + 4 < boardSize &&
            chessMap[row][col - i] == chessMap[row][col - i + 1] &&
            chessMap[row][col - i] == chessMap[row][col - i + 2] &&
            chessMap[row][col - i] == chessMap[row][col - i + 3] &&
            chessMap[row][col - i] == chessMap[row][col - i + 4])
            return true;
    }

    // 竖直方向(上下延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (row - i >= 0 &&
            row - i + 4 < boardSize &&
            chessMap[row - i][col] == chessMap[row - i + 1][col] &&
            chessMap[row - i][col] == chessMap[row - i + 2][col] &&
            chessMap[row - i][col] == chessMap[row - i + 3][col] &&
            chessMap[row - i][col] == chessMap[row - i + 4][col])
            return true;
    }

    // “/"方向
    for (int i = 0; i < 5; i++)
    {
        if (row + i < boardSize &&
            row + i - 4 >= 0 &&
            col - i >= 0 &&
            col - i + 4 < boardSize &&
            // 第[row+i]行，第[col-i]的棋子，与右上方连续4个棋子都相同
            chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
            chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
            chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
            chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
            return true;
    }

    // “\“ 方向
    for (int i = 0; i < 5; i++)
    {
        // 第[row+i]行，第[col-i]的棋子，与右下方连续4个棋子都相同
        if (row - i >= 0 &&
            row - i + 4 < boardSize &&
            col - i >= 0 &&
            col - i + 4 < boardSize &&
            chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] &&
            chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
            chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] &&
            chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4])
            return true;
    }

    return false;


    */
    int r = lastPos.row;
    int c = lastPos.col;
    int n = boardSize;
    qDebug() << "进入check_win";
    qDebug() << "win_r:" << r;
    qDebug() << "win_c:" << c;
    qDebug() << "win_n:" << n;
    constexpr static int dir[4][2] { {0, 1}, {1, -1}, {1, 1}, {1, 0} }; //设置四个主方位 右 右下 下 左下
    for (auto d : dir) {//一共20种可能
        //向右四个 向左一个右三个 左二右二 左三右一 右四个
        //正四
        if (0 <= r + d[0] && r + d[0] < n && 0 <= c + d[1] && c + d[1] < n
                && 0 <= r + 2 * d[0] && r + 2 * d[0] < n && 0 <= c + 2 * d[1] && c + 2 * d[1] < n
                && 0 <= r + 3 * d[0] && r + 3 * d[0] < n && 0 <= c + 3 * d[1] && c + 3 * d[1] < n
                && 0 <= r + 4 * d[0] && r + 4 * d[0] < n && 0 <= c + 4 * d[1] && c + 4 * d[1] < n
                && chessMap[r + d[0]][c + d[1]] == chessMap[r][c]
                && chessMap[r + 2 * d[0]][c + 2 * d[1]] == chessMap[r][c]
                && chessMap[r + 3 * d[0]][c + 3 * d[1]] == chessMap[r][c]
                && chessMap[r + 4 * d[0]][c + 4 * d[1]] == chessMap[r][c]) {
            return true;
        }
        //三正一负
        if (0 <= r + d[0] && r + d[0] < n && 0 <= c + d[1] && c + d[1] < n
                && 0 <= r + 2 * d[0] && r + 2 * d[0] < n && 0 <= c + 2 * d[1] && c + 2 * d[1] < n
                && 0 <= r + 3 * d[0] && r + 3 * d[0] < n && 0 <= c + 3 * d[1] && c + 3 * d[1] < n
                && 0 <= r - d[0] && r - d[0] < n && 0 <= c - d[1] && c - d[1] < n
                && chessMap[r + d[0]][c + d[1]] == chessMap[r][c]
                && chessMap[r + 2 * d[0]][c + 2 * d[1]] == chessMap[r][c]
                && chessMap[r + 3 * d[0]][c + 3 * d[1]] == chessMap[r][c]
                && chessMap[r - d[0]][c - d[1]] == chessMap[r][c]) {
            return true;
        }
        //2正2负
        if (0 <= r + d[0] && r + d[0] < n && 0 <= c + d[1] && c + d[1] < n
                && 0 <= r + 2 * d[0] && r + 2 * d[0] < n && 0 <= c + 2 * d[1] && c + 2 * d[1] < n
                && 0 <= r - 2 * d[0] && r - 2 * d[0] < n && 0 <= c - 2 * d[1] && c - 2 * d[1] < n
                && 0 <= r - d[0] && r - d[0] < n && 0 <= c - d[1] && c - d[1] < n
                && chessMap[r + d[0]][c + d[1]] == chessMap[r][c]
                && chessMap[r + 2 * d[0]][c + 2 * d[1]] == chessMap[r][c]
                && chessMap[r - 2 * d[0]][c - 2 * d[1]] == chessMap[r][c]
                && chessMap[r - d[0]][c - d[1]] == chessMap[r][c]) {
            return true;
        }//1正3负
        if (0 <= r + d[0] && r + d[0] < n && 0 <= c + d[1] && c + d[1] < n
                && 0 <= r - 3 * d[0] && r - 3 * d[0] < n && 0 <= c - 3 * d[1] && c - 3 * d[1] < n
                && 0 <= r - 2 * d[0] && r - 2 * d[0] < n && 0 <= c - 2 * d[1] && c - 2 * d[1] < n
                && 0 <= r - d[0] && r - d[0] < n && 0 <= c - d[1] && c - d[1] < n
                && chessMap[r + d[0]][c + d[1]] == chessMap[r][c]
                && chessMap[r - 3 * d[0]][c - 3 * d[1]] == chessMap[r][c]
                && chessMap[r - 2 * d[0]][c - 2 * d[1]] == chessMap[r][c]
                && chessMap[r - d[0]][c - d[1]] == chessMap[r][c]) {
            return true;
        }//4负
        if (0 <= r - 4 * d[0] && r - 4 * d[0] < n && 0 <= c - 4 * d[1] && c - 4 * d[1] < n
                && 0 <= r - 3 * d[0] && r - 3 * d[0] < n && 0 <= c - 3 * d[1] && c - 3 * d[1] < n
                && 0 <= r - 2 * d[0] && r - 2 * d[0] < n && 0 <= c - 2 * d[1] && c - 2 * d[1] < n
                && 0 <= r - d[0] && r - d[0] < n && 0 <= c - d[1] && c - d[1] < n
                && chessMap[r - 4 * d[0]][c - 4 * d[1]] == chessMap[r][c]
                && chessMap[r - 3 * d[0]][c - 3 * d[1]] == chessMap[r][c]
                && chessMap[r - 2 * d[0]][c - 2 * d[1]] == chessMap[r][c]
                && chessMap[r - d[0]][c - d[1]] == chessMap[r][c]) {
            return true;
        }
    }
    return false;
}

bool Chessboard::isOver()
{
    end_timer->stop();
    qDebug() << "检查游戏是否结束！";
    if (CheckWin()) {
        qDebug() << "游戏结束！";
        Sleep(2000);
        QImage over_img;
        QMediaPlayer *player1 = new QMediaPlayer;
        if (playerFlag == FALSE && FLAG == 0) {  //黑棋赢（玩家赢）,此时标记已经反转，轮到白棋落子 如果落子为FALSE代表上一个为TRUE即为黑子
            player1->setMedia(QUrl::fromLocalFile("./res/bucuo.mp3"));
            over_img = QImage("./res/vectory.jpg");
            ui->over_label->setPixmap(QPixmap("res/vectory.jpg"));
        } else if (playerFlag == TRUE && FLAG == 0) {
            player1->setMedia(QUrl::fromLocalFile("./res/defeat.mp3"));
            over_img = QImage("./res/defeat.jpg");
            ui->over_label->setPixmap(QPixmap("res/defeat.jpg"));
        } else if (playerFlag == FALSE && FLAG == 1) {  //黑棋赢（玩家赢）,此时标记已经反转，轮到白棋落子 如果落子为FALSE代表上一个为TRUE即为黑子
            player1->setMedia(QUrl::fromLocalFile("./res/defeat.mp3"));
            over_img = QImage("./res/defeat.jpg");
            ui->over_label->setPixmap(QPixmap("res/defeat.jpg"));
        } else if (playerFlag == TRUE && FLAG == 1) {
            player1->setMedia(QUrl::fromLocalFile("./res/bucuo.mp3"));
            over_img = QImage("./res/vectory.jpg");
            ui->over_label->setPixmap(QPixmap("res/vectory.jpg"));
        }
        player1->play();
        player->stop();
        player->stateChanged(QMediaPlayer::StoppedState);//设置停止播放
//        player_1->setVolume(0);
//        system("pause");
        ui->over_label->setGeometry(this->width() / 2 - over_img.width() / 2, this->height() / 2 - over_img.height() / 2,
                                    over_img.width(), over_img.height());
        ui->over_label->show();
        return true;
    }
    qDebug() << "游戏结束！！！！！！";
    return false;
}

