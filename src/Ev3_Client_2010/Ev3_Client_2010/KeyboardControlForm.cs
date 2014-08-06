using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EV3ControlClient
{
    public partial class KeyboardControlForm : Form , IControlForm
    {
        /// <summary>
        /// Returns the EV3ControlData instance
        /// </summary>
        public EV3ControlData EV3 { get { return ev3; } }
        private EV3ControlData ev3;

        /// <summary>
        /// true if EV3 ist Connected
        /// </summary>
        public bool Connected { get { return ev3.Connected; } }

        /// <summary>
        /// Returns EV3-error value
        /// </summary>
        public int Error { get { return ev3.Err; } }

        private int steps_speed = 4,
                    steps_turn = 2;


        public KeyboardControlForm(string ip)
        {
            InitializeComponent();
            ev3 = new EV3ControlData(ip, 4242);
            SetText();
            if (ev3.Err != 0 || !ev3.Connected)
            {
                MessageBox.Show("Could not initialise EV3\nError: " + ev3.Err);
                this.Close();
            }
        }


        /// <summary>
        /// Set text to GUI components
        /// </summary>
        private void SetText()
        {
            label_status.Text = "EV3-Status: " + ev3.Err;
            label_speed.Text = "Speed: " + ev3.drv.Speed;
            label_turn.Text = "Turn: " + ev3.drv.Turn;
            label_speedSteps.Text = steps_speed + " Steps";
            label_turnSteps.Text = steps_turn + " Steps";

            if (ev3.drv.brakes)
                label_brakes.Text = "Brakes: On";
            else
                label_brakes.Text = "Brakes: Off";

            if (ev3.drv.brake_mode)
                label_brake_mode.Text = "Brake mode: Brake";
            else
                label_brake_mode.Text = "Brake mode: Coast";

            if (ev3.drv.acc_steering)
                label_acc_steering.Text = "Accelerated Steering: On";
            else
                label_acc_steering.Text = "Accelerated Steering: Off";
        }

        /// <summary>
        /// Disconnect from server before closing window
        /// </summary>
        private void KeyboardControlForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            ev3.Disconnect();
        }



        /// <summary>
        /// Get and process key presses
        /// </summary>
        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            if (!e.Control)
            {
                switch (e.KeyValue)
                {
                    case 8:  // backspace
                        ev3.drv.brakes = false;
                        ev3.drv.Speed = 0;
                        ev3.drv.Turn = 0;
                        break;
                    case 32: // space
                        ev3.drv.brakes = !ev3.drv.brakes;
                        break;
                    case 33: // page up
                        steps_speed++;
                        break;
                    case 34: // page down
                        steps_speed--;
                        break;
                    case 36: // home
                        steps_turn++;
                        break;
                    case 35: // end
                        steps_turn--;
                        break;
                    case 37: // left 
                        ev3.drv.Turn -= steps_turn;
                        break;
                    case 39:  // right  
                        ev3.drv.Turn += steps_turn;
                        break;
                    case 38: // up
                        ev3.drv.Speed += steps_speed;
                        break;
                    case 40: // down  
                        ev3.drv.Speed -= steps_speed;
                        break;
                    case 45: // insert  
                        ev3.drv.brake_mode = !ev3.drv.brake_mode;
                        break;
                    case 46: // delete
                        ev3.drv.acc_steering = !ev3.drv.acc_steering;
                        break;
                }
                ev3.Send();
                SetText();
            }
        }

    }
}
