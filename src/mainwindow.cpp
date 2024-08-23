#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), startButton(new QPushButton("Start Journey", this)), loadButton(new QPushButton("Load Progress", this)), timer1(new QTimer(this))
{
    setFixedSize(1000, 700);
    timer1->setInterval(clk_ori);
    QObject::connect(timer1, &QTimer::timeout, this, [=]()
                     {
        emit check(); });
    timer1->start();
    Layers=nullptr;
    layered=nullptr;
    scrollArea=nullptr;
    layer_obj=nullptr;
    layer_moving=nullptr;
    layer_machinery=nullptr;
    layer_info=nullptr;
    map=nullptr;
    tools=nullptr;
    status=nullptr;
    filer=nullptr;
    music=new QMediaPlayer(this);
    audio=new QAudioOutput(this);
    audio->setVolume(0.1);
    music->setSource(QUrl("qrc:/bgms/bgms/bgm1.mp3"));
    music->setLoops(-1);
    music->setAudioOutput(audio);
}

MainWindow::~MainWindow()
{
    delete tools->upgrade_window;
    delete tools->guide_window;
}

void MainWindow::set_start()
{
    sign = new QLabel(this);
    sign->setGeometry(40, 50, 900, 500);
    sign->setPixmap(QPixmap(":/background/imgs/sign.png").scaled(900, 500));
    sign->show();
    startButton->setGeometry(350, 550, 300, 50);
    loadButton->setGeometry(350, 600, 300, 50);
    connect(startButton, SIGNAL(clicked()), this, SLOT(start()));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(load()));
}

void MainWindow::set_game(Tools *tools, Map *map)
{
    addToolBar(Qt::BottomToolBarArea, tools);
    map->setWindowOpacity(0.0);

    int n_row=map->n_row,n_col=map->n_col;

    bg=new QLabel(this);
    get_bg();
    bg->setGeometry(0,0,1000,650);
    bg->show();

    layer_obj = new LayerObject(n_row, n_col, map, this);
    layer_machinery = new LayerMachinery(n_row, n_col, map, this);
    layer_info = new LayerInfo(n_row, n_col, map, status, this);

    connect(tools->upgrade_window, &Upgrade::larger_receiver, map, &Map::larger_receiver);
    connect(tools->upgrade_window, &Upgrade::more_shapes, map, &Map::more_shapes);
    connect(tools->upgrade_window, &Upgrade::larger_map, this, &MainWindow::larger_map);
    connect(this, &MainWindow::check, layer_moving, &LayerMoving::check);
    connect(map,&Map::remove_moving,layer_moving,&LayerMoving::check_if_moving);
    connect(layer_moving, &LayerMoving::update_score, layer_info, &LayerInfo::update_layer_info_score);
    connect(status, &GameStatus::update_clk_sig, this, &MainWindow::update_clk);
    connect(status, &GameStatus::update_bg,this,&MainWindow::get_bg);
    connect(map, &Map::add_gen_item, layer_moving, &LayerMoving::add_gen_item);
    connect(map, &Map::remove_gen_item, layer_moving, &LayerMoving::remove_gen_item);

    Layers = new QStackedLayout;
    Layers->setStackingMode(QStackedLayout::StackAll);

    Layers->addWidget(layer_obj);
    Layers->addWidget(layer_moving);
    Layers->addWidget(layer_machinery);
    Layers->addWidget(layer_info);
    Layers->addWidget(map);

    layered = new QWidget(this);
    layered->setLayout(Layers);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(layered);
    scrollArea->setWidgetResizable(true);
    setCentralWidget(scrollArea);

    int hCenter = (scrollArea->horizontalScrollBar()->maximum() - scrollArea->horizontalScrollBar()->minimum()) / 2;
    int vCenter = (scrollArea->verticalScrollBar()->maximum() - scrollArea->verticalScrollBar()->minimum()) / 2;
    scrollArea->horizontalScrollBar()->setValue(hCenter);
    scrollArea->verticalScrollBar()->setValue(vCenter);

    music->play();
}

void MainWindow::start()
{
    sign->clear();
    disconnect(startButton, SIGNAL(clicked()), this, SLOT(start()));
    startButton->hide();
    disconnect(loadButton, SIGNAL(clicked()), this, SLOT(load()));
    loadButton->hide();

    status = new GameStatus();
    tools = new Tools(status, this);
    map = new Map(ROW, COL, this);
    layer_moving = new LayerMoving(ROW, COL, map, status, clk_ori, this);
    filer = new Filing(map, status, layer_moving);

    connect(tools, &Tools::save_file, filer, &Filing::save_file);
    set_game(tools, map);
}

void MainWindow::load()
{
    sign->clear();
    disconnect(startButton, SIGNAL(clicked()), this, SLOT(start()));
    startButton->hide();
    disconnect(loadButton, SIGNAL(clicked()), this, SLOT(load()));
    loadButton->hide();

    QString directoryPath ="./UserFile/";
    QDir().mkpath(directoryPath);
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        "Load File",
        directoryPath,
        "Text Files (*.txt)"
        );
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file";
        return;
    }
    QTextStream in(&file);
    QString line=in.readLine();
    while(!line.startsWith("Map")){
        line=in.readLine();
    }
    line=in.readLine();
    QStringList mapsize = line.split("*");
    int n_row=mapsize[0].toInt(),n_col=mapsize[1].toInt();
    file.close();

    status = new GameStatus();
    tools = new Tools(status, this);
    map = new Map(n_row, n_col, this);
    layer_moving = new LayerMoving(n_row, n_col, map, status, clk_ori, this);
    filer = new Filing(map, status, layer_moving);

    connect(tools, &Tools::save_file, filer, &Filing::save_file);

    filer->load_file(fileName);
    update_clk(status->ani_rate);
    set_game(tools, map);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_R)
    {
        // Perform the action when "R" is pressed
        if (cursor().shape() != Qt::ArrowCursor)
        {
            map->tool_dir = (map->tool_dir + 1) % 4;
            QPixmap pic = cursor().pixmap();
            QTransform transform;
            transform.rotate(90);
            pic = pic.transformed(transform);
            setCursor(QCursor(pic));
        }
    }
    else if (event->key() == Qt::Key_S)
    {
        map->stick=!map->stick;
    }
    else
    {
        // Call base class implementation for other keys
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::update_clk(int r)
{
    timer1->setInterval(clk_ori / r);
}

void MainWindow::larger_map()
{
    int sx=map->n_row;
    int sy=map->n_col;
    Map *map_temp=new Map(sx*1.5, sy*1.5, this);
    for(int i=0;i<sx;i++)
        for(int j=0;j<sy;j++)
        {
            map_temp->map[i+sx/4][j+sy/4]->tool_type=map->map[i][j]->tool_type;
            map_temp->map[i+sx/4][j+sy/4]->tool_dir=map->map[i][j]->tool_dir;
            map_temp->map[i+sx/4][j+sy/4]->tool_state=map->map[i][j]->tool_state;
            map_temp->map[i+sx/4][j+sy/4]->shape_type=map->map[i][j]->shape_type;
        }
    map_temp->center_size=map->center_size;
    LayerMoving *layer_moving_temp = new LayerMoving(sx*1.5, sy*1.5,map_temp, status, clk_ori, this);
    for(int i=0;i<sx;i++)
        for(int j=0;j<sy;j++)
        {
            layer_moving_temp->map_moving[i+sx/4][j+sy/4]=layer_moving->map_moving[i][j];
        }
    for(auto i:layer_moving->gen_items){
        layer_moving_temp->add_gen_item(i.x+sx/4,i.y+sy/4,i.type,i.dir,i.gen_state);
    }
    for(auto i:layer_moving->moving_items){
        layer_moving_temp->add_moving_item(i.x+sx/4,i.y+sy/4,i.type,i.dir);
    }
    LayerObject *layer_obj_temp = new LayerObject(sx*1.5, sy*1.5, map_temp, this);
    LayerMachinery *layer_machinery_temp = new LayerMachinery(sx*1.5, sy*1.5, map_temp, this);
    LayerInfo *layer_info_temp = new LayerInfo(sx*1.5, sy*1.5, map_temp, status, this);

    Layers->replaceWidget(layer_obj, layer_obj_temp);
    Layers->replaceWidget(layer_moving, layer_moving_temp);
    Layers->replaceWidget(layer_machinery, layer_machinery_temp);
    Layers->replaceWidget(layer_info, layer_info_temp);
    Layers->replaceWidget(map, map_temp);

    disconnect(tools->upgrade_window, &Upgrade::larger_receiver, map, &Map::larger_receiver);
    disconnect(tools->upgrade_window, &Upgrade::more_shapes, map, &Map::more_shapes);
    disconnect(this, &MainWindow::check, layer_moving, &LayerMoving::check);
    disconnect(layer_moving, &LayerMoving::update_score, layer_info, &LayerInfo::update_layer_info_score);
    disconnect(map,&Map::remove_moving,layer_moving,&LayerMoving::check_if_moving);
    disconnect(map, &Map::add_gen_item, layer_moving, &LayerMoving::add_gen_item);
    disconnect(map, &Map::remove_gen_item, layer_moving, &LayerMoving::remove_gen_item);

    delete map;
    map=map_temp;
    delete layer_moving;
    layer_moving=layer_moving_temp;
    delete layer_obj;
    layer_obj=layer_obj_temp;
    delete layer_machinery;
    layer_machinery=layer_machinery_temp;
    delete layer_info;
    layer_info=layer_info_temp;

    disconnect(tools, &Tools::save_file, filer, &Filing::save_file);
    delete filer;
    filer = new Filing(map, status, layer_moving);
    connect(tools, &Tools::save_file, filer, &Filing::save_file);
    connect(tools->upgrade_window, &Upgrade::larger_receiver, map, &Map::larger_receiver);
    connect(tools->upgrade_window, &Upgrade::more_shapes, map, &Map::more_shapes);
    connect(this, &MainWindow::check, layer_moving, &LayerMoving::check);
    connect(layer_moving, &LayerMoving::update_score, layer_info, &LayerInfo::update_layer_info_score);
    connect(map,&Map::remove_moving,layer_moving,&LayerMoving::check_if_moving);
    connect(map, &Map::add_gen_item, layer_moving, &LayerMoving::add_gen_item);
    connect(map, &Map::remove_gen_item, layer_moving, &LayerMoving::remove_gen_item);
}

void MainWindow::get_bg()
{
    switch (status->level%3) {
    case 0:
        bg->setPixmap(QPixmap(":/background/imgs/bg1.png").scaled(1000,700));
        break;
    case 1:
        bg->setPixmap(QPixmap(":/background/imgs/bg2.png").scaled(1000,700));
        break;
    case 2:
        bg->setPixmap(QPixmap(":/background/imgs/bg3.png").scaled(1000,700));
        break;
    default:
        break;
    }
}
