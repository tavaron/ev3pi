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
    public partial class JoystickControlForm : Form , IControlForm
    {
        public EV3ControlData EV3 { get { return ev3; } }
        private EV3ControlData ev3;

        public bool Connected { get { return ev3.Connected; } }

        public int Error { get { return ev3.Err; } }
        
        public JoystickControlForm(string ip)
        {
            InitializeComponent();
            ev3 = new EV3ControlData(ip, 4242);
            if (ev3.Err != 0)
            {
                MessageBox.Show("Could not initialise EV3\nError: " + ev3.Err);
                this.Close();
            }
            SetText();
        }

        private void SetText()
        {
            label_status.Text = "EV3-Status: " + ev3.Err;
            label_speed.Text = "Speed: " + ev3.drv.Speed;
            label_turn.Text = "Turn: " + ev3.drv.Turn;

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

        
    }
}
