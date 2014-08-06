/* Error codes
#define EV3_OK                      0
#define EV3_NOT_PRESENT             1
#define EV3_CONFIGURATION_ERROR     2
#define EV3_IN_USE                  3
#define EV3_USB_WRITE_ERROR         4
#define EV3_USB_READ_ERROR          5
#define EV3_USB_CONNECTION_CLOSED	6
#define EV3_USB_WAIT_TO_CONNECT		7
#define EV3_NOT_INITIALIZED	        8 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Windows.Forms;



namespace EV3ControlClient
{

    public class EV3ControlData : IDisposable
    {
       

        public drive_t drv = new drive_t();
        private TcpClient tcpClient;
        private Stream networkStream;

        private byte err = 8;
        public byte Err { get { return err; } }

        private bool connected = false;
        public bool Connected { get { return connected; } }


	    public EV3ControlData(string ip_str, Int32 port)
	    {
            Connect(ip_str, port);
	    }

        public EV3ControlData()
        {
            connected = false;
        }


        public int Connect(string ip_str, Int32 port)
        {
            int error = 2;
            try
            {
                tcpClient = new TcpClient();
                tcpClient.Connect(ip_str, port);
                networkStream = tcpClient.GetStream();
                error--;

                connected = true;
                Send(0);
                error--;
            }
            catch
            {
                MessageBox.Show("Could not connect to " + ip_str + ":" + port + "\nError: " + error);
                connected = false;
            }
            
            return error;
        }


        public void Disconnect()
        {
            Disconnect(true);
            
        }

        private void Disconnect(bool send)
        {
            if (send)
                Send(255);

            connected = false;
            networkStream.Dispose();
            networkStream = null;
            tcpClient.Close();
            tcpClient = null;
        }

        // Send struct
        public bool Send(byte cmd)
        {
            if (!connected)
                return false;

            byte[] send = GetBytes(drv);
            byte[] recv = new byte[send.Length];

            int error = 2;

            send[send.Length - 1] = cmd;

            try
            {
                networkStream.Write(send, 0, send.Length);
                error--;

                networkStream.Read(recv, 0, recv.Length);
                error--;

                ProcServerMsg(recv);

            }
            catch
            {
                connected = false;
            }

            if (err == 255)
            {
                Disconnect(false);
            }

            return connected;
        }

        public bool Send()
        {
            return Send(0);
        }

        public static byte Send(byte cmd, string ip, int port)
        {
            byte[] recv = new byte[20];
            byte[] send = new byte[20];
            byte err = 3;
            for (int i = 0; i < send.Length-1; i++)
            {
                send[i] = 0;
            }
            send[19] = cmd;

            TcpClient tcpClient;
            Stream networkStream;

            try
            {
                tcpClient = new TcpClient();
                tcpClient.Connect(ip, port);
                networkStream = tcpClient.GetStream();
                err--;

                networkStream.Write(send, 0, send.Length);
                err--;

                networkStream.Close();
                tcpClient.Close();
                err--;
            }
            catch { }

            return err;
        }

        private byte ProcServerMsg(byte[] b)
        {
            err = b[0];

            switch (b[19])
            {
                case 255:
                    Disconnect(false);
                    break;
            }

            return b[19];
        }


        public string GetByteString()
        {
            string s = ByteArrayToString(GetBytes(drv));
            return s;
        }

        public static byte[] GetBytes(drive_t drive)
        {
            int size = Marshal.SizeOf(drive);
            byte[] arr = new byte[size];
            IntPtr ptr = Marshal.AllocHGlobal(size);

            Marshal.StructureToPtr(drive, ptr, true);
            Marshal.Copy(ptr, arr, 0, size);
            Marshal.FreeHGlobal(ptr);

            return arr;
        }

        public byte[] GetBytes()
        {
            byte[] arr = GetBytes(drv);
            return arr;
        }

        public static string ByteArrayToString(byte[] ba)
        {
            StringBuilder hex = new StringBuilder(ba.Length * 2);
            foreach (byte b in ba)
                hex.AppendFormat("{0:x2} ", b);
            return hex.ToString();
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this); 
        }

        protected virtual void Dispose(bool disposing)
        {
            try
            {
                if (disposing)
                {
                    if(networkStream != null)
                        networkStream.Dispose();
                    if (tcpClient != null)
                        tcpClient.Close();
                }
            }
            catch { }
            networkStream = null;
            tcpClient = null;
            connected = false;

        }
    }
}
