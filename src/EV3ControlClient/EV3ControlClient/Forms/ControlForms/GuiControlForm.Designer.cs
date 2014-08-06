namespace EV3ControlClient
{
    partial class GuiControlForm
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
            this.label_speed = new System.Windows.Forms.Label();
            this.label_acc_steering = new System.Windows.Forms.Label();
            this.label_brake_mode = new System.Windows.Forms.Label();
            this.label_turn = new System.Windows.Forms.Label();
            this.button_acc_steering = new System.Windows.Forms.Button();
            this.button_brake_mode = new System.Windows.Forms.Button();
            this.button_brakes = new System.Windows.Forms.Button();
            this.tb_speed = new System.Windows.Forms.TextBox();
            this.tb_turn = new System.Windows.Forms.TextBox();
            this.button_send = new System.Windows.Forms.Button();
            this.label_status = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // label_speed
            // 
            this.label_speed.AutoSize = true;
            this.label_speed.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_speed.Location = new System.Drawing.Point(102, 13);
            this.label_speed.Name = "label_speed";
            this.label_speed.Size = new System.Drawing.Size(49, 16);
            this.label_speed.TabIndex = 0;
            this.label_speed.Text = "Speed";
            // 
            // label_acc_steering
            // 
            this.label_acc_steering.AutoSize = true;
            this.label_acc_steering.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_acc_steering.Location = new System.Drawing.Point(17, 97);
            this.label_acc_steering.Name = "label_acc_steering";
            this.label_acc_steering.Size = new System.Drawing.Size(134, 16);
            this.label_acc_steering.TabIndex = 1;
            this.label_acc_steering.Text = "Accelerated Steering";
            // 
            // label_brake_mode
            // 
            this.label_brake_mode.AutoSize = true;
            this.label_brake_mode.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_brake_mode.Location = new System.Drawing.Point(69, 68);
            this.label_brake_mode.Name = "label_brake_mode";
            this.label_brake_mode.Size = new System.Drawing.Size(82, 16);
            this.label_brake_mode.TabIndex = 2;
            this.label_brake_mode.Text = "Brake Mode";
            // 
            // label_turn
            // 
            this.label_turn.AutoSize = true;
            this.label_turn.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_turn.Location = new System.Drawing.Point(116, 40);
            this.label_turn.Name = "label_turn";
            this.label_turn.Size = new System.Drawing.Size(35, 16);
            this.label_turn.TabIndex = 4;
            this.label_turn.Text = "Turn";
            // 
            // button_acc_steering
            // 
            this.button_acc_steering.Location = new System.Drawing.Point(157, 94);
            this.button_acc_steering.Name = "button_acc_steering";
            this.button_acc_steering.Size = new System.Drawing.Size(115, 23);
            this.button_acc_steering.TabIndex = 5;
            this.button_acc_steering.UseVisualStyleBackColor = true;
            this.button_acc_steering.Click += new System.EventHandler(this.button_acc_steering_Click);
            // 
            // button_brake_mode
            // 
            this.button_brake_mode.Location = new System.Drawing.Point(157, 65);
            this.button_brake_mode.Name = "button_brake_mode";
            this.button_brake_mode.Size = new System.Drawing.Size(115, 23);
            this.button_brake_mode.TabIndex = 6;
            this.button_brake_mode.UseVisualStyleBackColor = true;
            this.button_brake_mode.Click += new System.EventHandler(this.button_brake_mode_Click);
            // 
            // button_brakes
            // 
            this.button_brakes.Location = new System.Drawing.Point(12, 126);
            this.button_brakes.Name = "button_brakes";
            this.button_brakes.Size = new System.Drawing.Size(139, 23);
            this.button_brakes.TabIndex = 7;
            this.button_brakes.UseVisualStyleBackColor = true;
            this.button_brakes.Click += new System.EventHandler(this.button_brakes_Click);
            // 
            // tb_speed
            // 
            this.tb_speed.Location = new System.Drawing.Point(157, 12);
            this.tb_speed.Name = "tb_speed";
            this.tb_speed.Size = new System.Drawing.Size(115, 20);
            this.tb_speed.TabIndex = 8;
            // 
            // tb_turn
            // 
            this.tb_turn.Location = new System.Drawing.Point(157, 39);
            this.tb_turn.Name = "tb_turn";
            this.tb_turn.Size = new System.Drawing.Size(115, 20);
            this.tb_turn.TabIndex = 9;
            // 
            // button_send
            // 
            this.button_send.Location = new System.Drawing.Point(157, 126);
            this.button_send.Name = "button_send";
            this.button_send.Size = new System.Drawing.Size(115, 23);
            this.button_send.TabIndex = 10;
            this.button_send.Text = "Senden";
            this.button_send.UseVisualStyleBackColor = true;
            this.button_send.Visible = false;
            this.button_send.Click += new System.EventHandler(this.button_send_Click);
            // 
            // label_status
            // 
            this.label_status.AutoSize = true;
            this.label_status.Location = new System.Drawing.Point(12, 152);
            this.label_status.Name = "label_status";
            this.label_status.Size = new System.Drawing.Size(72, 13);
            this.label_status.TabIndex = 12;
            this.label_status.Text = "EV3-Status: 8";
            // 
            // GuiControlForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 169);
            this.Controls.Add(this.label_speed);
            this.Controls.Add(this.label_acc_steering);
            this.Controls.Add(this.label_brake_mode);
            this.Controls.Add(this.label_turn);
            this.Controls.Add(this.label_status);
            this.Controls.Add(this.button_acc_steering);
            this.Controls.Add(this.button_brake_mode);
            this.Controls.Add(this.button_send);
            this.Controls.Add(this.button_brakes);
            this.Controls.Add(this.tb_turn);
            this.Controls.Add(this.tb_speed);
            this.Name = "GuiControlForm";
            this.Text = "GuiControl";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.GuiControlForm_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label_speed;
        private System.Windows.Forms.Label label_acc_steering;
        private System.Windows.Forms.Label label_brake_mode;
        private System.Windows.Forms.Label label_turn;
        private System.Windows.Forms.Button button_acc_steering;
        private System.Windows.Forms.Button button_brake_mode;
        private System.Windows.Forms.Button button_brakes;
        private System.Windows.Forms.TextBox tb_speed;
        private System.Windows.Forms.TextBox tb_turn;
        private System.Windows.Forms.Button button_send;
        private System.Windows.Forms.Label label_status;
    }
}