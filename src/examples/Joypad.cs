using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;
using SharpDX.DirectInput;
using System.Windows.Forms;


namespace nxtGamepadLogger
{
    public class Joypad
    {
        // Testswitch
        bool testSwitch = false;
        // Fields
        private Joystick joystick;      // SharpDX.Joystick

        // Properties
        public int Ahead { get; set; }
        public int Turn { get; set; }
        public double AheadGain { get; set; }
        public double TurnGain { get; set; }
        public bool SwitchEnableRecord { get; set; }

        public Joypad()
        {
            // Initialize DirectInput
            var directInput = new DirectInput();

            // Find a Joystick Guid
            var joystickGuid = Guid.Empty;

            foreach (var deviceInstance in directInput.GetDevices(DeviceType.Gamepad,
                        DeviceEnumerationFlags.AllDevices))
                joystickGuid = deviceInstance.InstanceGuid;

            // If Gamepad not found, look for a Joystick
            if (joystickGuid == Guid.Empty)
                foreach (var deviceInstance in directInput.GetDevices(DeviceType.Joystick,
                        DeviceEnumerationFlags.AllDevices))
                    joystickGuid = deviceInstance.InstanceGuid;

            // If Joystick not found, throws an error
            if (joystickGuid == Guid.Empty)
            {
                MessageBox.Show("No joystick/Gamepad found.", "Joypad");
                Console.ReadKey();
                Environment.Exit(1);
            }

            // Instantiate the joystick
            joystick = new Joystick(directInput, joystickGuid);

            string output;
            output = String.Format("Found Joystick/Gamepad with GUID: {0}", joystickGuid);
            
            // Query all suported ForceFeedback effects
            var allEffects = joystick.GetEffects();
            
            foreach (var effectInfo in allEffects)
                output += String.Format("\nEffect available {0}", effectInfo.Name);
      
            if (testSwitch) CustomMessageBox.Show("Joypad Messages", output);

            // Set BufferSize in order to use buffered data.
            joystick.Properties.BufferSize = 128;

            // Acquire the joystick
            joystick.Acquire();

            // Initialize Joypad Gains
            AheadGain = -100;
            TurnGain = -50;
            SwitchEnableRecord = false;
        }

        public void poll()
        {
            bool test = false;                          // deactivate test mode

            // Poll events from joystick
            joystick.Poll();

            var datas = joystick.GetBufferedData();
            foreach (var state in datas)
            {
                if (test)
                {
                    Console.WriteLine(state);
                    Console.WriteLine((int)state.Offset);
                }
                if ((int)state.Offset == 4)             // linker Joystick, vor/zurück
                {
                    Ahead = (int)((state.Value / 32768.0 - 1) * AheadGain);
                    if(test) Console.WriteLine("Straight: {0,4}", Ahead);
                }
                if ((int)state.Offset == 20)            // rechter Joystick, links/rechts
                {
                    Turn = (int)((state.Value / 32768.0 - 1) * TurnGain);
                    if(test) Console.WriteLine("Turn    : {0,4}", Turn);
                }
                if ((int)state.Offset == 48)            // Button 1, EnableRecord
                {
                    if (state.Value > 0) SwitchEnableRecord = true;
                    if(test) Console.WriteLine("Record  : {0,4}", SwitchEnableRecord);
                }
            }
        }
    }
}
    