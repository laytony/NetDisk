#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QLabel>
#include <QMediaPlayer>
#include <QWidget>
#include <unordered_map>

using namespace std;

struct ChessPos { //表征落子位置 行列
    int row;
    int col;
    //构造函数
    ChessPos(int r = 0, int c = 0) : row(r), col(c) {}
};

typedef enum {//定义枚举类型，定义棋的颜色分类
    CHESS_WHITE = -1,//白子
    CHESS_BLACK = 1//黑子
} Chess_kind;

namespace Ui
{
    class Chessboard;
}

class Chessboard : public QWidget
{
    Q_OBJECT

public:
    explicit Chessboard(QWidget *parent = nullptr);
    ~Chessboard();

protected:
    void closeEvent(QCloseEvent *event);//关闭窗口触发关闭音乐

private:
    Ui::Chessboard *ui;
    QImage chess_img;
    QImage blackChess_img;//黑色棋子图标,使用图形库
    QImage whiteChess_img;//白色棋子图标
    int boardSize;//定义棋盘大小 13 15 17 19线
    int margin_x;//棋盘横向边缘宽度//
    int margin_y;//棋盘纵向边缘宽度
    float chess_size;//棋子大小 与 棋盘方格相等

    //定义修改棋盘状态的函数 定义为私有的
    //定义下棋方
    bool playerFlag;//true:黑方 false:白
    ChessPos lastPos; //最近落子位置, Chess的private数据成员
    void updatechessmap(ChessPos *chesspos);

public:
    int mouse_x_pose;
    int mouse_y_pose;
//    unordered_map<pair<int, int>, QLabel *>chess_label;
    unordered_map<int, QLabel *>chess_label;

    vector<vector<int>>chessMap;//定义棋盘状态0 无子 1 黑子 -1 白子
    //构造函数
    void i_Chessboard(int boardSize, int margin_x, int margin_y, float chess_size, int flag); //初始化需要用的参数即可

    //棋盘初始化函数
    void Init(int size);//初始化为多少线的棋盘

    //获取棋盘大小（13,15,17,19线大小）
    int getBoardsize();

    //判断点击棋盘位置是否是一个有效地点击 如果是有效地点击返回棋盘上点击确定的有效位置
    bool is_validchessdown(int x, int y, ChessPos *pos);

    //在指定位置下子
    void chessDown(ChessPos *pos, Chess_kind kind);

    //获取指定位置是黑棋 白旗 还是空白 两个重载函数
    int getChessData(ChessPos *pos);
    int getChessData(int row, int col);
    //判断棋局是否赢了
    bool CheckWin();
    //检查棋局是否结束
    bool isOver();

    void mousePressEvent(QMouseEvent *event);

    bool eventFilter(QObject *object, QEvent *event) override;//回车重新开始


    //每种类型对应的评分
    ///AI走棋类
    ///
    ///
    ///
    vector<vector<float>>scoreMap;//每个点的概率 评分
    QTimer *ai_timer;
    QTimer *end_timer;
    //AI的行为 下子
    int FLAG;//标记先手还是后手 0是先手
    QMediaPlayer *player;
    QMediaPlaylist *plist;
public slots:
    void AI_opration();


    ///实现AI走棋的思路：
    ///棋理格言：敌之好点，即我之好点。
    ///就是说，每个点，都要考虑，如果敌方占领了这个点，会产生多大的价值，如果我方占领了这个点，又会产生多大的价值。如果我方占领这个点，
    ///价值只有1000，但是敌方要是占领了这个点，价值有2000，而在自己在其它位置没有价值更高的点，那么建议直接抢占这个敌方的好点。

    ///AI先计算棋手如果在这个位置落子，会有多大的价值。然后再计算自己如果在这个位置落子，有大大价值。具体计算方法，就是计算如果黑棋或者白棋在这个位置落子
    ///，那么在这个位置的某个方向上， 一共有连续几个黑子或者连续几个白子。连续的数量越多，价值越大
    /// 总价值之和为八个方向上的和
    ///
    /// 初始化棋盘得分 越靠近中间得分越高 最边缘不加分 按圈加1 2 3 ，，，至中间 12*12格
    /// （敌方下完  待计算位置的 得分） 下这个点的评分 等于我方的下这个点的得分 + 阻值敌方下该点的得分; 设置为防守为主为主
    /// 得分    敌方     我方     情况
    ///         10       10      连2
    ///         30       25      死连3
    ///         50       55      活连3
    ///         70       65      死连4
    ///         300      10000   活连4
    ///         20000    30000   连5
    ///
    ///
    ///
    void inint_scoremap();
    ///获取权值矩阵
    void AIgetscore();

    bool is_validpos(int r, int c);

    void AIgetscore_ray();

    ChessPos AIthink();//模拟AI思考或许下棋位置

    float getsinglescore(vector<int> &state);

signals:
    void ai_go_signal();

};


#endif // CHESSBOARD_H
