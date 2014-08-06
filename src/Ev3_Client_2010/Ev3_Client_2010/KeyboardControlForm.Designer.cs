namespace EV3ControlClient
{
    partial class KeyboardControlForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
                ev3.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label_status = new System.Windows.Forms.Label();
            this.label_speed = new System.Windows.Forms.Label();
            this.label_turn = new System.Windows.Forms.Label();
            this.label_brakes = new System.Windows.Forms.Label();
            this.label_speedSteps = new System.Windows.Forms.Label();
            this.label_turnSteps = new System.Windows.Forms.Label();
            this.label_brake_mode = new System.Windows.Forms.Label();
            this.label_acc_steering = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label_status
            // 
            this.label_status.AutoSize = true;
            this.label_status.Location = new System.Drawing.Point(12, 9);
            this.label_status.Name = "label_status";
            this.label_status.Size = new System.Drawing.Size(72, 13);
            this.label_status.TabIndex = 14;
            this.label_status.Text = "EV3-Status: 8";
            // 
            // label_speed
            // 
            this.label_speed.AutoSize = true;
            this.label_speed.Location = new System.Drawing.Point(12, 35);
            this.label_speed.Name = "label_speed";
            this.label_speed.Size = new System.Drawing.Size(50, 13);
            this.label_speed.TabIndex = 16;
            this.label_speed.Text = "Speed: 0";
            // 
            // label_turn
            // 
            this.label_turn.AutoSize = true;
            this.label_turn.Location = new System.Drawing.Point(12, 48);
            this.label_turn.Name = "label_turn";
            this.label_turn.Size = new System.Drawing.Size(41, 13);
            this.label_turn.TabIndex = 17;
            this.label_turn.Text = "Turn: 0";
            // 
            // label_brakes
            // 
            this.label_brakes.AutoSize = true;
            this.label_brakes.Location = new System.Drawing.Point(12, 61);
            this.label_brakes.Name = "label_brakes";
            this.label_brakes.Size = new System.Drawing.Size(60, 13);
            this.label_brakes.TabIndex = 18;
            this.label_brakes.Text = "Brakes: Off";
            // 
            // label_speedSteps
            // 
            this.label_speedSteps.AutoSize = true;
            this.label_speedSteps.Location = new System.Drawing.Point(79, 35);
            this.label_speedSteps.Name = "label_speedSteps";
            this.label_speedSteps.Size = new System.Drawing.Size(43, 13);
            this.label_speedSteps.TabIndex = 19;
            this.label_speedSteps.Text = "4 Steps";
            // 
            // label_turnSteps
            // 
            this.label_turnSteps.AutoSize = true;
            this.label_turnSteps.Location = new System.Drawing.Point(79, 48);
            this.label_turnSteps.Name = "label_turnSteps";
            this.label_turnSteps.Size = new System.Drawing.Size(43, 13);
            this.label_turnSteps.TabIndex = 20;
            this.label_turnSteps.Text = "2 Steps";
            // 
            // label_brake_mode
            // 
            this.label_brake_mode.AutoSize = true;
            this.label_brake_mode.Location = new System.Drawing.Point(12, 74);
            this.label_brake_mode.Name = "label_brake_mode";
            this.label_brake_mode.Size = new System.Drawing.Size(97, 13);
            this.label_brake_mode.TabIndex = 21;
            this.label_brake_mode.Text = "Brake mode: Coast";
            // 
            // label_acc_steering
            // 
            this.label_acc_steering.AutoSize = true;
            this.label_acc_steering.Location = new System.Drawing.Point(12, 87);
            this.label_acc_steering.Name = "label_acc_steering";
            this.label_acc_steering.Size = new System.Drawing.Size(126, 13);
            this.label_acc_steering.TabIndex = 22;
            this.label_acc_steering.Text = "Accelerated Steering: Off";
            // 
            // KeyboardControlForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(159, 109);
            this.Controls.Add(this.label_acc_steering);
            this.Controls.Add(this.label_brake_mode);
            this.Controls.Add(this.label_turnSteps);
            this.Controls.Add(this.label_speedSteps);
            this.Controls.Add(this.label_brakes);
            this.Controls.Add(this.label_turn);
            this.Controls.Add(this.label_speed);
            this.Controls.Add(this.label_status);
            this.Name = "KeyboardControlForm";
            this.Text = "KeyboardControlForm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.KeyboardControlForm_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label_status;
        private System.Windows.Forms.Label label_speed;
        private System.Windows.Forms.Label label_turn;
        private System.Windows.Forms.Label label_brakes;
        private System.Windows.Forms.Label label_speedSteps;
        private System.Windows.Forms.Label label_turnSteps;
        private System.Windows.Forms.Label label_brake_mode;
        private System.Windows.Forms.Label label_acc_steering;

    }
}