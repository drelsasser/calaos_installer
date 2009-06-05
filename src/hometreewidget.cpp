#include "hometreewidget.h"
#include "mainwindow.h"
#include <ListeRoom.h>

HomeTreeWidget::HomeTreeWidget(QWidget *parent):
                QTreeWidget(parent)
{
        setAcceptDrops(true);
        setDragEnabled(true);
}

bool HomeTreeWidget::dropMimeData(QTreeWidgetItem *parent, int, const QMimeData *data, Qt::DropAction)
{
        QList<QUrl> urlList;
        QFileInfo info;
        QString fName;

        urlList = data->urls(); // retrieve list of urls

        foreach(QUrl url, urlList) // iterate over list
        {
                fName = url.toLocalFile();
                info.setFile( fName );

                string id = info.fileName().toLocal8Bit().data();
                
                if (parent != NULL)
                {
                        QTreeWidgetItemRoom *pitem = dynamic_cast<QTreeWidgetItemRoom *>(parent);
                        if (pitem)
                        {
                                Input *input = ListeRoom::Instance().get_input(id);
                                if (input)
                                {
                                        int _i = ListeRoom::Instance().searchIO(input);
                                        if (_i < 0) return false;
                                        Room *old_room = ListeRoom::Instance().get_room(_i);

                                        if (pitem->getRoom() == old_room) return false;

                                        //delete old item
                                        old_room->RemoveInput(input);

                                        QTreeWidgetItemIterator it(this);
                                        while (*it)
                                        {
                                                QTreeWidgetItemInput *item = dynamic_cast<QTreeWidgetItemInput *>(*it);
                                                if (item && item->getInput() == input)
                                                {
                                                        delete item;
                                                        break;
                                                }
                                                ++it;
                                        }

                                        //create new item
                                        MainWindow *win = dynamic_cast<MainWindow *>(parentWidget());
                                        win->addItemInput(input, pitem);
                                        pitem->getRoom()->AddInput(input);

                                        return true;
                                }

                                Output *output = ListeRoom::Instance().get_output(id);
                                if (output)
                                {
                                        int _i = ListeRoom::Instance().searchIO(output);
                                        if (_i < 0) return false;
                                        Room *old_room = ListeRoom::Instance().get_room(_i);

                                        if (pitem->getRoom() == old_room) return false;

                                        //delete old item
                                        old_room->RemoveOutput(output);

                                        QTreeWidgetItemIterator it(this);
                                        while (*it)
                                        {
                                                QTreeWidgetItemOutput *item = dynamic_cast<QTreeWidgetItemOutput *>(*it);
                                                if (item && item->getOutput() == output)
                                                {
                                                        delete item;
                                                        break;
                                                }
                                                ++it;
                                        }

                                        MainWindow *win = dynamic_cast<MainWindow *>(parentWidget());
                                        win->addItemOutput(output, pitem);
                                        pitem->getRoom()->AddOutput(output);

                                        return true;
                                }
                        }
                }             
        }

        return true;
}

QStringList HomeTreeWidget::mimeTypes () const
{
        QStringList l;

        // list of accepted mime types for drop
        l.append("text/uri-list");

        return l;
}

Qt::DropActions HomeTreeWidget::supportedDropActions () const
{
        return Qt::CopyAction | Qt::MoveAction;
}

void HomeTreeWidget::mousePressEvent(QMouseEvent *event)
{
        QTreeWidget::mousePressEvent(event);

        if(event->button() == Qt::LeftButton)
                dragStartPosition = event->pos();
}

void HomeTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
        if (!(event->buttons() & Qt::LeftButton))
                return;
        if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
                return;
        if (currentItem() == NULL)
                return;

        if (this->itemAt(QPoint(event->x(), event->y())) != currentItem())
                return;

        //check if current item is an Input/Output and not a room
        QTreeWidgetItemRoom *item = dynamic_cast<QTreeWidgetItemRoom *>(currentItem());
        if (item)
                return;

        QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(currentItem());
        if (itinput)
        {
                QDrag *drag = new QDrag(this);
                QMimeData *mimeData = new QMimeData;
                QList<QUrl> list;
                QString qs = QString::fromUtf8(itinput->getInput()->get_param("id").c_str());
                list.append(QUrl(qs));

                // mime stuff
                mimeData->setUrls(list);
                drag->setMimeData(mimeData);

                // start drag
                drag->start(Qt::CopyAction | Qt::MoveAction);
        }

        QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(currentItem());
        if (itoutput)
        {
                QDrag *drag = new QDrag(this);
                QMimeData *mimeData = new QMimeData;
                QList<QUrl> list;
                QString qs = QString::fromUtf8(itoutput->getOutput()->get_param("id").c_str());
                list.append(QUrl(qs));

                // mime stuff
                mimeData->setUrls(list);
                drag->setMimeData(mimeData);

                // start drag
                drag->start(Qt::CopyAction | Qt::MoveAction);
        }
}
