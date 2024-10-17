#include <QGraphicsBlurEffect>
#include "GameScene.h"
#define SHILLING 100
#define HEALTH 20
GameScene::GameScene(int level, bool isHardMode, QGraphicsView *parent)
    : QGraphicsView(parent)
{
    this->level = level;
    this->isHardMode = isHardMode;
    //this->player = new Player(SHILLING, HEALTH);
    this->map = new Map();
    pauseGameButton = new Button("路径1", "路径2", 1000, 700);
    scene = new QGraphicsScene(this);
    scene->addItem(pauseGameButton);
    pauseGameButton->setZValue(90);

    gameEndButton = new Button("路径1", "路径2", 1100, 700);
    scene->addItem(gameEndButton);
    gameEndButton->setZValue(90);

    resumeGameButton = nullptr;
    pausedMenu = nullptr;

    healthTextItem = new QGraphicsTextItem(QString("HEALTH: %1").arg(HEALTH));
    scene->addItem(healthTextItem);
    healthTextItem->setPos(1000, 0);
    healthTextItem->setDefaultTextColor(Qt::red);
    healthTextItem->setZValue(90);

    moneyTextItem = new QGraphicsTextItem(QString("SHILLING: %1").arg(SHILLING));
    moneyTextItem->setPos(1000, 100);
    moneyTextItem->setDefaultTextColor(Qt::yellow);
    healthTextItem->setZValue(90);

    towerSelectMenu = nullptr;

    QTimer * updateTimer = new QTimer(this);
    updateTimer->start(50);
    connect(updateTimer, &QTimer::timeout, this, &GameScene::updateScene);

    loadMap(level);
}



void GameScene::onPauseButtonClicked()
{
    pauseScene();
    if (!pausedMenu)
    {
        pausedMenu = new QGraphicsRectItem(QRectF(scene->sceneRect()));
        pausedMenu->setBrush(QBrush(QColor(255, 255, 255, 128)));
        pausedMenu->setZValue(100);

        QGraphicsBlurEffect *pauseMenuEffect = new QGraphicsBlurEffect();
        pauseMenuEffect->setBlurRadius(100);
        pausedMenu->setGraphicsEffect(pauseMenuEffect);
        scene->addItem(pausedMenu);
        resumeGameButton = new Button("路径1", "路径2", 550, 350);
        resumeGameButton->setParentItem(pausedMenu);
        resumeGameButton->setZValue(101);
        scene->addItem(resumeGameButton);
    }
}

void GameScene::onResumeButtonClicked()
{
    resumeScene();
    if (pausedMenu)
    {
        scene->removeItem(pausedMenu);
        scene->removeItem(resumeGameButton);
        delete pausedMenu;
        delete resumeGameButton;
        pausedMenu = nullptr;
        resumeGameButton = nullptr;
    }
}

void GameScene::addTower(TowerFrame *tower)
{
    scene->addItem(tower);
    tower->setZValue(3);
    towers.append(tower);
}

void GameScene::addEnemy()
{
    Enemy* enemy = new Enemy(map->getPath,map->getEnemyTypes());
    scene->addItem(enemy);
    enemy->setZValue(1);
    enemies.append(enemy);
}

void GameScene::addObstacles()
{
    QVector<QVector<int>> obsPos = map->getObsPosType();
    for(int i=0;i<obsPos.size();i++)
    {
        Obstacle* obstacle = new Obstacle(QPoint(i[0],i[1]),i[2]);
        scene->addItem(obstacle);
        obstacle->setZValue(2);
        obstacles.append(obstacle);
    }
}

void GameScene::updateScene()
{
    scene->update();
}

void GameScene::pauseScene()
{
    for(Enemy* i:enemies)
    {
        i->enemyPause();
    }
    for(TowerFrame* i :towers)
    {
        i->towerPause();
    }
}
void GameScene::resumeScene()
{
    for(Enemy* i:enemies)
    {
        i->enemyResume();
    }
    for(TowerFrame* i :towers)
    {
        i->towerResume();
    }
}
void GameScene::mousePressEvent(QMouseEvent* event)
{
    QPointF pressPos = event->pos();
    QPoint pressPosInt = pressPos.toPoint();
    if(map->isPlaceAble(pressPosInt))
    {
        pauseScene();
        towerSelectMenu = new TowerSelectMenu(pressPosInt);
        scene->addItem(towerSelectMenu);
        towerSelectMenu->setZValue(95);
        connect(towerSelectMenu,&TowerSelectMenu::selectTowerType,this,&GameScene::onTowerSelectButtonClicked);
        connect(towerSelectMenu, &::TowerSelectMenu::closeTowerSelectMenu,[=](){
            scene->removeItem(towerSelectMenu);
            delete towerSelectMenu;
        });

    }
}

void GameScene::closeEvent(QCloseEvent *event)
{
    emit gameEnd();
    event->accept();
}

void GameScene::loadMap(int level)
{
    map->loadMap(level);
    addObstacles();
    //todo
    //背景怎么办
}
