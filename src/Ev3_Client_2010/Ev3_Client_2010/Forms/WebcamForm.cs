using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using MjpegProcessor;

namespace EV3ControlClient
{
    public partial class WebcamForm : Form
    {
        private MjpegDecoder mjpegDec;
        private Bitmap img = null;
        private bool connected = false;
        private System.Threading.Timer timer;
        private Uri uri;

        public WebcamForm(string ip)
        {
            InitializeComponent();
            mjpegDec = new MjpegDecoder();
            mjpegDec.FrameReady += mjpeg_FrameReady;
            uri = new Uri("http://" + ip + ":8080/");
            label_url.Text = uri.ToString();
            SetText();
        }

        private void Webcam_FormClosing(object sender, FormClosingEventArgs e)
        {
            mjpegDec.StopStream();
        }
        
        private void mjpeg_FrameReady(object sender, FrameReadyEventArgs e)
        {
            img = new Bitmap(e.Bitmap);
            pb_mjpeg.Image = img;
        }

        private void SetText()
        {
            if (connected)
            {
                button_connect.Text = "Disconnect";
                button_save.Visible = true;
            }
            else
            {
                button_connect.Text = "Connect";
                button_save.Visible = false;
            }
        }
        
        private void button_connect_Click(object sender, EventArgs e)
        {
            try
            {
                if (!connected)
                {
                    mjpegDec.ParseStream(uri);
                    connected = true;
                }
                else
                {
                    mjpegDec.StopStream();
                    connected = false;
                }
            }
            catch { }
            SetText();
        }

        private void button_reset_Click(object sender, EventArgs e)
        {
            int i = 1;
            mjpegDec.StopStream();
            connected = false;
            // Server Command for restarting motion deamon
            int err = EV3ControlData.Send(4, uri.Host, 4242);

            switch (err)
            {
                case 0:
                    button_connect.Text = "Connecting... " + i + "/3";
                    button_connect.Enabled = false;
                    timer = new System.Threading.Timer(ConnectCallback, i, 8000, Timeout.Infinite);
                    break;
                default:
                    MessageBox.Show("Reset Fehlgeschlagen\nFehler: " + err);
                    break;
            }
            

        }


        private void ConnectCallback( Object state )
        {
            int i = (int)state;
            
            try
            {
                mjpegDec.ParseStream(uri);
                connected = true;
                
                if (this.InvokeRequired)
                    this.BeginInvoke(new Action(() =>
                    {
                        button_connect.Enabled = true;
                        SetText();
                    }));
                else
                {
                    button_connect.Enabled = true;
                    SetText();
                }
            }
            catch
            {
                if (i < 3)
                {
                    i++;
                    if (button_connect.InvokeRequired)
                        button_connect.BeginInvoke( new Action(() => 
                            { button_connect.Text = "Connecting... " + i + "/3"; }));
                    else
                        button_connect.Text = "Connecting... " + i + "/3";
                    timer = new System.Threading.Timer(ConnectCallback, i, 4000, Timeout.Infinite);
                }
                else
                {
                    MessageBox.Show("Verbindung mit " + uri.Host + " fehlgeschlagen");
                    connected = false;
                    button_connect.Enabled = true;
                    i = 0;
                }
                    
            }
        }

        private void button_save_Click(object sender, EventArgs e)
        {

        }

    }
}
