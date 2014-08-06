namespace EV3ControlClient
{
    partial class WebcamForm
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
                timer.Dispose();
                img.Dispose();
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
            this.button_reset = new System.Windows.Forms.Button();
            this.button_connect = new System.Windows.Forms.Button();
            this.pb_mjpeg = new System.Windows.Forms.PictureBox();
            this.button_save = new System.Windows.Forms.Button();
            this.label_url = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pb_mjpeg)).BeginInit();
            this.SuspendLayout();
            // 
            // button_reset
            // 
            this.button_reset.Location = new System.Drawing.Point(252, 319);
            this.button_reset.Name = "button_reset";
            this.button_reset.Size = new System.Drawing.Size(114, 23);
            this.button_reset.TabIndex = 26;
            this.button_reset.Text = "Reset Server";
            this.button_reset.UseVisualStyleBackColor = true;
            this.button_reset.Click += new System.EventHandler(this.button_reset_Click);
            // 
            // button_connect
            // 
            this.button_connect.Location = new System.Drawing.Point(132, 319);
            this.button_connect.Name = "button_connect";
            this.button_connect.Size = new System.Drawing.Size(114, 23);
            this.button_connect.TabIndex = 24;
            this.button_connect.Text = "Connect";
            this.button_connect.UseVisualStyleBackColor = true;
            this.button_connect.Click += new System.EventHandler(this.button_connect_Click);
            // 
            // pb_mjpeg
            // 
            this.pb_mjpeg.Location = new System.Drawing.Point(12, 25);
            this.pb_mjpeg.Name = "pb_mjpeg";
            this.pb_mjpeg.Size = new System.Drawing.Size(354, 288);
            this.pb_mjpeg.TabIndex = 22;
            this.pb_mjpeg.TabStop = false;
            // 
            // button_save
            // 
            this.button_save.Location = new System.Drawing.Point(12, 319);
            this.button_save.Name = "button_save";
            this.button_save.Size = new System.Drawing.Size(114, 23);
            this.button_save.TabIndex = 28;
            this.button_save.Text = "Save Image";
            this.button_save.UseVisualStyleBackColor = true;
            // 
            // label_url
            // 
            this.label_url.AutoSize = true;
            this.label_url.Location = new System.Drawing.Point(12, 9);
            this.label_url.Name = "label_url";
            this.label_url.Size = new System.Drawing.Size(38, 13);
            this.label_url.TabIndex = 29;
            this.label_url.Text = "URL: -";
            // 
            // WebcamForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(378, 352);
            this.Controls.Add(this.label_url);
            this.Controls.Add(this.button_save);
            this.Controls.Add(this.button_reset);
            this.Controls.Add(this.button_connect);
            this.Controls.Add(this.pb_mjpeg);
            this.Name = "WebcamForm";
            this.Text = "Webcam";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Webcam_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.pb_mjpeg)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button_reset;
        private System.Windows.Forms.Button button_connect;
        private System.Windows.Forms.PictureBox pb_mjpeg;
        private System.Windows.Forms.Button button_save;
        private System.Windows.Forms.Label label_url;
    }
}