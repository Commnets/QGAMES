using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace Retrospec.IsoConverter
{
	/// <summary>
	/// Summary description for images.
	/// </summary>
	public class images : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button ButtonClose;
		private GlacialComponents.Controls.GlacialList glImages;
		private System.Windows.Forms.ImageList ilItems;
		private System.ComponentModel.IContainer components;

		public images()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

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
			this.components = new System.ComponentModel.Container();
			GlacialComponents.Controls.GLColumn glColumn1 = new GlacialComponents.Controls.GLColumn();
			GlacialComponents.Controls.GLColumn glColumn2 = new GlacialComponents.Controls.GLColumn();
			GlacialComponents.Controls.GLColumn glColumn3 = new GlacialComponents.Controls.GLColumn();
			this.glImages = new GlacialComponents.Controls.GlacialList();
			this.ButtonClose = new System.Windows.Forms.Button();
			this.ilItems = new System.Windows.Forms.ImageList(this.components);
			this.SuspendLayout();
			// 
			// glImages
			// 
			this.glImages.AllowColumnResize = true;
			this.glImages.AllowMultiselect = false;
			this.glImages.AlternateBackground = System.Drawing.Color.DarkGreen;
			this.glImages.AlternatingColors = false;
			this.glImages.AutoHeight = true;
			this.glImages.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.glImages.BackgroundStretchToFit = true;
			glColumn1.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn1.CheckBoxes = false;
			glColumn1.ImageIndex = -1;
			glColumn1.Name = "Column2";
			glColumn1.NumericSort = false;
			glColumn1.Text = "Block";
			glColumn1.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn1.Width = 70;
			glColumn2.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn2.CheckBoxes = false;
			glColumn2.ImageIndex = -1;
			glColumn2.Name = "Column3";
			glColumn2.NumericSort = false;
			glColumn2.Text = "Name";
			glColumn2.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn2.Width = 270;
			glColumn3.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn3.CheckBoxes = false;
			glColumn3.ImageIndex = -1;
			glColumn3.Name = "Column4";
			glColumn3.NumericSort = false;
			glColumn3.Text = "Type";
			glColumn3.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn3.Width = 100;
			this.glImages.Columns.AddRange(new GlacialComponents.Controls.GLColumn[] {
																						 glColumn1,
																						 glColumn2,
																						 glColumn3});
			this.glImages.ControlStyle = GlacialComponents.Controls.GLControlStyles.Normal;
			this.glImages.FullRowSelect = true;
			this.glImages.GridColor = System.Drawing.Color.LightGray;
			this.glImages.GridLines = GlacialComponents.Controls.GLGridLines.gridBoth;
			this.glImages.GridLineStyle = GlacialComponents.Controls.GLGridLineStyles.gridSolid;
			this.glImages.GridTypes = GlacialComponents.Controls.GLGridTypes.gridOnExists;
			this.glImages.HeaderHeight = 22;
			this.glImages.HeaderVisible = true;
			this.glImages.HeaderWordWrap = false;
			this.glImages.HotColumnTracking = false;
			this.glImages.HotItemTracking = false;
			this.glImages.HotTrackingColor = System.Drawing.Color.LightGray;
			this.glImages.HoverEvents = false;
			this.glImages.HoverTime = 1;
			this.glImages.ImageList = null;
			this.glImages.ItemHeight = 17;
			this.glImages.ItemWordWrap = false;
			this.glImages.Location = new System.Drawing.Point(8, 16);
			this.glImages.Name = "glImages";
			this.glImages.Selectable = true;
			this.glImages.SelectedTextColor = System.Drawing.Color.White;
			this.glImages.SelectionColor = System.Drawing.Color.DarkBlue;
			this.glImages.ShowBorder = true;
			this.glImages.ShowFocusRect = false;
			this.glImages.Size = new System.Drawing.Size(440, 480);
			this.glImages.SortType = GlacialComponents.Controls.SortTypes.InsertionSort;
			this.glImages.SuperFlatHeaderColor = System.Drawing.Color.White;
			this.glImages.TabIndex = 0;
			this.glImages.Text = "glacialList1";
			// 
			// ButtonClose
			// 
			this.ButtonClose.Location = new System.Drawing.Point(376, 496);
			this.ButtonClose.Name = "ButtonClose";
			this.ButtonClose.Size = new System.Drawing.Size(75, 32);
			this.ButtonClose.TabIndex = 1;
			this.ButtonClose.Text = "&Close";
			this.ButtonClose.Click += new System.EventHandler(this.ButtonClose_Click);
			// 
			// ilItems
			// 
			this.ilItems.ImageSize = new System.Drawing.Size(64, 64);
			this.ilItems.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// images
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(458, 530);
			this.Controls.Add(this.ButtonClose);
			this.Controls.Add(this.glImages);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "images";
			this.Text = "Game Images";
			this.ResumeLayout(false);

		}
		#endregion

		private void ButtonClose_Click(object sender, System.EventArgs e)
		{
			this.Close();
		}

		public void DoList(IsoData idata)
		{
			this.ilItems.Images.Clear();
			this.glImages.Items.Clear();
			this.glImages.ImageList=ilItems;
			foreach(BlockDescriptor bd in idata.DefinedRooms)
			{
				if(bd.bmp==null) continue;
				try 
				{
					if(bd.bmp!="")
						this.ilItems.Images.Add(new Bitmap("images\\"+bd.bmp));
				} 
				catch 
				{
					this.ilItems.Images.Add(new Bitmap("images\\unknown.gif"));
					System.Windows.Forms.MessageBox.Show(this,"Could not open bitmap: " + "images\\"+bd.bmp);
				}

				GlacialComponents.Controls.GLItem gli=new GlacialComponents.Controls.GLItem();
				GlacialComponents.Controls.GLSubItem gls;
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.ImageIndex=(this.ilItems.Images.Count-1);
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=bd.Name; //name
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text="Room";
				gli.SubItems.Add(gls);

				this.glImages.Items.Add(gli);
			}

			foreach(BlockDescriptor bd in idata.DefinedBlocks)
			{
				if(bd.bmp==null) continue;
				try 
				{
					if(bd.bmp!="")
						this.ilItems.Images.Add(new Bitmap("images\\"+bd.bmp));
				} 
				catch 
				{
					this.ilItems.Images.Add(new Bitmap("images\\unknown.gif"));
					System.Windows.Forms.MessageBox.Show(this,"Could not open bitmap: " + "images\\"+bd.bmp);
				}

				GlacialComponents.Controls.GLItem gli=new GlacialComponents.Controls.GLItem();
				GlacialComponents.Controls.GLSubItem gls;
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.ImageIndex=(this.ilItems.Images.Count-1);
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=bd.Name; //name
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text="Object";
				gli.SubItems.Add(gls);

				this.glImages.Items.Add(gli);
			}

			int ilasttype=-1;
			foreach(PlacedObject bd in idata.PlacedObjects)
			{
				if(bd.Type!=ilasttype)
				{
					if(bd.bmp==null) continue;
					try 
					{
						if(bd.bmp!="")
							this.ilItems.Images.Add(new Bitmap("images\\"+bd.bmp));
					} 
					catch 
					{
						this.ilItems.Images.Add(new Bitmap("images\\unknown.gif"));
						System.Windows.Forms.MessageBox.Show(this,"Could not open bitmap: " + "images\\"+bd.bmp);
					}

					GlacialComponents.Controls.GLItem gli=new GlacialComponents.Controls.GLItem();
					GlacialComponents.Controls.GLSubItem gls;
					gls=new GlacialComponents.Controls.GLSubItem();
					gls.ImageIndex=(this.ilItems.Images.Count-1);
					gli.SubItems.Add(gls);

					gls=new GlacialComponents.Controls.GLSubItem();
					gls.Text=bd.Name; //name
					gli.SubItems.Add(gls);

					gls=new GlacialComponents.Controls.GLSubItem();
					gls.Text="Object";
					gli.SubItems.Add(gls);

					this.glImages.Items.Add(gli);
					ilasttype=bd.Type;
				}
			}

			this.glImages.Refresh();
		}
	}
}
