using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;

namespace Retrospec.IsoConverter
{
	/// <summary>
	/// Summary description for outputwindow.
	/// </summary>
	public class outputwindow : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox OutputMain;
		private System.Windows.Forms.Button ButtonClose;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public outputwindow()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.OutputMain = new System.Windows.Forms.TextBox();
			this.ButtonClose = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// OutputMain
			// 
			this.OutputMain.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.OutputMain.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.OutputMain.ForeColor = System.Drawing.SystemColors.HotTrack;
			this.OutputMain.Location = new System.Drawing.Point(8, 8);
			this.OutputMain.Multiline = true;
			this.OutputMain.Name = "OutputMain";
			this.OutputMain.ReadOnly = true;
			this.OutputMain.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.OutputMain.Size = new System.Drawing.Size(624, 512);
			this.OutputMain.TabIndex = 34;
			this.OutputMain.Text = "displays stuff...";
			this.OutputMain.WordWrap = false;
			// 
			// ButtonClose
			// 
			this.ButtonClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.ButtonClose.Location = new System.Drawing.Point(560, 528);
			this.ButtonClose.Name = "ButtonClose";
			this.ButtonClose.TabIndex = 35;
			this.ButtonClose.Text = "&Close";
			this.ButtonClose.Click += new System.EventHandler(this.ButtonClose_Click);
			// 
			// outputwindow
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.ButtonClose;
			this.ClientSize = new System.Drawing.Size(642, 559);
			this.Controls.Add(this.ButtonClose);
			this.Controls.Add(this.OutputMain);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Name = "outputwindow";
			this.ResumeLayout(false);

		}
		#endregion

		private void ButtonClose_Click(object sender, System.EventArgs e)
		{
			this.Close();
		}

		public bool LoadData(string filetoload)
		{
			StreamReader sr;
			string srdata;
			try
			{
				sr=new StreamReader(filetoload);
				srdata=sr.ReadToEnd();
				sr.Close();
				this.OutputMain.Text=srdata;
				this.OutputMain.SelectionLength=0;
				return true;
			}
			catch
			{
				MessageBox.Show(this,"Could not read the map file output '"+filetoload+"' Does it exist? Has the map been loaded and converted?");
				return false;
			}
		}
	}
}
