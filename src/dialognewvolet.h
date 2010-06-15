#ifndef DIALOGNEWVOLET_H
#define DIALOGNEWVOLET_H

#include <QtGui/QDialog>
#include <iostream>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
        class DialogNewVolet;
}

class DialogNewVolet : public QDialog
{
                Q_OBJECT
                Q_DISABLE_COPY(DialogNewVolet)
        public:
                explicit DialogNewVolet(Room *room, QWidget *parent = 0);
                virtual ~DialogNewVolet();

                Output *getOutput() { return volet; }

        protected:
                virtual void changeEvent(QEvent *e);

        private:
                Ui::DialogNewVolet *ui;
                string type;
                Output *volet;
                Room *room;

        private slots:
                void on_check_smart_stateChanged(int );
                void on_buttonBox_accepted();
};

#endif // DIALOGNEWROOM_H