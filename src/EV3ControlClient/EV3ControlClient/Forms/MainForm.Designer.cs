namespace EV3ControlClient
{
    partial class MainForm
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.tb_ipaddr = new System.Windows.Forms.TextBox();
            this.button_gui = new System.Windows.Forms.Button();
            this.button_webcam = new System.Windows.Forms.Button();
            this.label_controls = new System.Windows.Forms.Label();
            this.button_joystick = new System.Windows.Forms.Button();
            this.button_keyboard = new System.Windows.Forms.Button();
            this.label_ip = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // tb_ipaddr
            // 
            this.tb_ipaddr.Location = new System.Drawing.Point(12, 34);
            this.tb_ipaddr.Name = "tb_ipaddr";
            this.tb_ipaddr.Size = new System.Drawing.Size(79, 20);
            this.tb_ipaddr.TabIndex = 14;
            this.tb_ipaddr.Text = "192.168.0.42";
            // 
            // button_gui
            // 
            this.button_gui.Location = new System.Drawing.Point(104, 32);
            this.button_gui.Name = "button_gui";
            this.button_gui.Size = new System.Drawing.Size(79, 23);
            this.button_gui.TabIndex = 15;
            this.button_gui.Text = "GUI";
            this.button_gui.UseVisualStyleBackColor = true;
            this.button_gui.Click += new System.EventHandler(this.button_gui_Click);
            // 
            // button_webcam
            // 
            this.button_webcam.Location = new System.Drawing.Point(12, 90);
            this.button_webcam.Name = "button_webcam";
            this.button_webcam.Size = new System.Drawing.Size(79, 23);
            this.button_webcam.TabIndex = 19;
            this.button_webcam.Text = "Webcam";
            this.button_webcam.UseVisualStyleBackColor = true;
            this.button_webcam.Click += new System.EventHandler(this.button_webcam_Click);
            // 
            // label_controls
            // 
            this.label_controls.AutoSize = true;
            this.label_controls.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_controls.Location = new System.Drawing.Point(104, 9);
            this.label_controls.Name = "label_controls";
            this.label_controls.Size = new System.Drawing.Size(68, 20);
            this.label_controls.TabIndex = 20;
            this.label_controls.Text = "Controls";
            // 
            // button_joystick
            // 
            this.button_joystick.Location = new System.Drawing.Point(104, 90);
            this.button_joystick.Name = "button_joystick";
            this.button_joystick.Size = new System.Drawing.Size(79, 23);
            this.button_joystick.TabIndex = 21;
            this.button_joystick.Text = "Joystick";
            this.button_joystick.UseVisualStyleBackColor = true;
            this.button_joystick.Click += new System.EventHandler(this.button_joystick_Click);
            // 
            // button_keyboard
            // 
            this.button_keyboard.Location = new System.Drawing.Point(104, 61);
            this.button_keyboard.Name = "button_keyboard";
            this.button_keyboard.Size = new System.Drawing.Size(79, 23);
            this.button_keyboard.TabIndex = 22;
            this.button_keyboard.Text = "Keyboard";
            this.button_keyboard.UseVisualStyleBackColor = true;
            this.button_keyboard.Click += new System.EventHandler(this.button_keyboard_Click);
            // 
            // label_ip
            // 
            this.label_ip.AutoSize = true;
            this.label_ip.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Underline, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_ip.Location = new System.Drawing.Point(8, 9);
            this.label_ip.Name = "label_ip";
            this.label_ip.Size = new System.Drawing.Size(24, 20);
            this.label_ip.TabIndex = 23;
            this.label_ip.Text = "IP";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(195, 132);
            this.Controls.Add(this.label_ip);
            this.Controls.Add(this.button_keyboard);
            this.Controls.Add(this.button_joystick);
            this.Controls.Add(this.label_controls);
            this.Controls.Add(this.button_webcam);
            this.Controls.Add(this.button_gui);
            this.Controls.Add(this.tb_ipaddr);
            this.Name = "MainForm";
            this.Text = "Main Menu";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox tb_ipaddr;
        private System.Windows.Forms.Button button_gui;
        private System.Windows.Forms.Button button_webcam;
        private System.Windows.Forms.Label label_controls;
        private System.Windows.Forms.Button button_joystick;
        private System.Windows.Forms.Button button_keyboard;
        private System.Windows.Forms.Label label_ip;
    }
}

