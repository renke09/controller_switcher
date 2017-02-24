/**
 * @file /include/controller_switcher/main_window.hpp
 *
 * @brief Qt based gui for controller_switcher.
 *
 * @date November 2010
 **/
#ifndef controller_switcher_MAIN_WINDOW_H
#define controller_switcher_MAIN_WINDOW_H

/*****************************************************************************
 ** Includes
 *****************************************************************************/

#include <QtGui/QMainWindow>

#ifndef Q_MOC_RUN
#include "ui_main_window.h"
#include "qnode.hpp"
#endif

/*****************************************************************************
 ** Namespace
 *****************************************************************************/

namespace controller_switcher {

  /*****************************************************************************
   ** Interface [MainWindow]
   *****************************************************************************/
  /**
   * @brief Qt central, all operations relating to the view part here.
   */
  class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(int argc, char** argv, QWidget *parent = 0);
    ~MainWindow();

  private:
    void fill_controllers_list();
    void fill_controllers_command_fields();
    void fill_cartpos_command_fields();
    void fill_hybrid_command_fields();
    void fill_sensor_configuration();
    void field_error_msg_box(std::string field_name);
    void service_error_msg_box(std::string controller_name);

  public Q_SLOTS:
    /******************************************
     ** Auto-connections (connectSlotsByName())
     *******************************************/
    void on_buttonQuit_clicked(bool check);
    void on_buttonSet_hybrid_clicked(bool check);
    void on_buttonSet_cartpos_clicked(bool check);
    void on_buttonReload_cartpos_clicked(bool check);
    void on_buttonEstimateTool_ftsensor_clicked(bool check);
    void on_buttonSetBias_ftsensor_clicked(bool check);
    void on_buttonCalibrate_ftsensor_clicked(bool check);
    void on_buttonSwitch_clicked(bool check);

    /******************************************
     ** manual connection
     *******************************************/
    void updateJointsState();
    void updateJointsError();
    void updateCartesianError();

  private:
    Ui::MainWindowDesign ui;
    QNode qnode;
  };

}  // namespace controller_switcher

#endif // controller_switcher_MAIN_WINDOW_H
