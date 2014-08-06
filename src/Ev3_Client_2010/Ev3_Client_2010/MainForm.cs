using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MjpegProcessor;

namespace EV3ControlClient
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }


        private WebcamForm webcam = null;
        private IControlForm control = null;


        private void button_webcam_Click(object sender, EventArgs e)
        {
			/* close existing webcam windows */
            if (webcam != null)
                webcam.Close();
            try
            {
				/* open new webcam window and pass ip from textbox */
                webcam = new WebcamForm(tb_ipaddr.Text);
                webcam.Show();
            }
            catch { }
        }


        private void button_gui_Click(object sender, EventArgs e)
        {
			/* close existing control windows */
            if (control != null)
                control.Close();
                
            try
            {
				/* open new gui control window and pass ip from textbox */
                control = new GuiControlForm(tb_ipaddr.Text);
                if(control != null)
                    control.Show();

            }
            catch { }
        }

        private void button_keyboard_Click(object sender, EventArgs e)
        {
			/* close existing control windows */
            if (control != null)
                control.Close();
                
            try
            {
				/* open new keyboard control window and pass ip from textbox */
                control = new KeyboardControlForm(tb_ipaddr.Text);
                if (control != null)
                    control.Show();
            }
            catch { }
        }

        private void button_joystick_Click(object sender, EventArgs e)
        {
			/* close existing control windows */
            if (control != null)
                control.Close();
            try
            {
				/* open new joystick control window and pass ip from textbox */
                control = new JoystickControlForm(tb_ipaddr.Text);
                if (control != null)
                    control.Show();
            }
            catch { }
        }

    }
}
