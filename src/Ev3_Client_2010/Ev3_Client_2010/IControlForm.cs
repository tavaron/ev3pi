using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EV3ControlClient
{
    public interface IControlForm
    {
        EV3ControlData EV3 { get; }
        bool Connected { get; }
        int Error { get; }
        void Show();
        void Hide();
        void Close();
        bool Visible { get; set; }
        
    }
}
