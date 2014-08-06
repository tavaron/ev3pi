using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EV3ControlClient
{
    public struct drive_t
    {
        private Int32 speed;
        public Int32 Speed {
            set
            {
                if (value >= -100 && value <= 100)
                    this.speed = value;
                else if (value < -100)
                    this.speed = -100;
                else
                    this.speed = 100;
            }

            get
            {
                return this.speed;
            }
        }

        private Int32 turn;
        public Int32 Turn {
            set
            {
                if (value >= -100 && value <= 100)
                    this.turn = value;
                else if (value < -100)
                    this.turn = -100;
                else
                    this.turn = 100;
            }
            get
            {
                return this.turn;
            }
        }

        public bool brakes,
                    brake_mode,
                    acc_steering;
    };
}
