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
    
    public partial class GuiControlForm : Form , IControlForm
    {
        public EV3ControlData EV3 { get { return ev3; } }
        private EV3ControlData ev3;

        public bool Connected { get { return ev3.Connected; } }

        public int Error { get { return ev3.Err; } }

        


        public GuiControlForm(string ip)
        {
            InitializeComponent();
            ev3 = new EV3ControlData(ip,4242);
            SetText();
            if (ev3.Err != 0 || !ev3.Connected)
            {
                MessageBox.Show("Could not initialise EV3\nError: " + ev3.Err);
                this.Close();
            }
        }

        private void SetText()
        {
            tb_speed.Text = Convert.ToString(ev3.drv.Speed);
            tb_turn.Text = Convert.ToString(ev3.drv.Turn);
            button_brakes.Text = "Brakes: " + Convert.ToString(ev3.drv.brakes);
            button_brake_mode.Text = Convert.ToString(ev3.drv.brake_mode);
            button_acc_steering.Text = Convert.ToString(ev3.drv.acc_steering);
            label_status.Text = "EV3-Status: " + ev3.Err;

            if (ev3.Connected)
                button_send.Visible = true;
            else
                button_send.Visible = false;
        }



        private void GuiControlForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            ev3.Disconnect();
        }

        private void button_send_Click(object sender, EventArgs e)
        {
            ev3.drv.Speed = Convert.ToInt32(tb_speed.Text);
            ev3.drv.Turn = Convert.ToInt32(tb_turn.Text);
            ev3.drv.brakes = false;
            ev3.Send();
            SetText();
        }

        private void button_brake_mode_Click(object sender, EventArgs e)
        {
            ev3.drv.brake_mode = !ev3.drv.brake_mode;
            SetText();
        }

        private void button_acc_steering_Click(object sender, EventArgs e)
        {
            ev3.drv.acc_steering = !ev3.drv.acc_steering;
            SetText();
        }

        /// <summary>
        /// Set brakes active and send command
        /// </summary>
        private void button_brakes_Click(object sender, EventArgs e)
        {
            ev3.drv.brakes = true;
            ev3.Send();
            SetText();
        }



        
    }

    
}
