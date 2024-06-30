using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;
using GlacialComponents;

namespace Retrospec.IsoConverter
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ContextMenu MapLoader;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private IsoData myData;
		private System.Windows.Forms.ImageList ImageListFG;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.ToolBar ToolbarMain;
		private System.Windows.Forms.ToolBarButton ButtonLoad;
		private System.Windows.Forms.ToolBarButton ButtonHelp;
		private System.Windows.Forms.ToolBarButton toolBarButton1;
		private System.Windows.Forms.ToolBarButton ButtonExit;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.RadioButton RadioXML;
		private System.Windows.Forms.RadioButton RadioDebug;
		private System.Windows.Forms.Label LabelAttr;
		private System.Windows.Forms.Label LabelID;
		private System.Windows.Forms.ListBox ListboxRooms;
		private System.Windows.Forms.Label LabelRooms;
		private GlacialComponents.Controls.GlacialList glList;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label label13;
		private System.Windows.Forms.ImageList ImageListSP;
		private System.Windows.Forms.ToolBarButton ButtonItems;
		private System.Windows.Forms.MenuItem MenuAtic;
		private System.Windows.Forms.Label LabelRoomLinks;
		private System.Windows.Forms.ListBox ListBoxBaddies;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ListBox ListBoxACGKey;
		private System.Windows.Forms.ListBox ListBoxGreen;
		private System.Windows.Forms.ListBox ListBoxRed;
		private System.Windows.Forms.ListBox ListBoxCyan;
		private System.Windows.Forms.ListBox ListBoxYellow;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label8;
		private System.ComponentModel.IContainer components;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			SplashScreen ss=new SplashScreen(false);
			ss.TopLevel=true;
			ss.Show();
			Application.DoEvents();
			System.Threading.Thread.Sleep(3000);
			ss.Close();
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
			myData=new IsoData();

			StartUpRoom();

		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
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
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(MainForm));
			GlacialComponents.Controls.GLColumn glColumn1 = new GlacialComponents.Controls.GLColumn();
			GlacialComponents.Controls.GLColumn glColumn2 = new GlacialComponents.Controls.GLColumn();
			GlacialComponents.Controls.GLColumn glColumn3 = new GlacialComponents.Controls.GLColumn();
			GlacialComponents.Controls.GLColumn glColumn4 = new GlacialComponents.Controls.GLColumn();
			GlacialComponents.Controls.GLColumn glColumn5 = new GlacialComponents.Controls.GLColumn();
			GlacialComponents.Controls.GLColumn glColumn6 = new GlacialComponents.Controls.GLColumn();
			GlacialComponents.Controls.GLColumn glColumn7 = new GlacialComponents.Controls.GLColumn();
			GlacialComponents.Controls.GLColumn glColumn8 = new GlacialComponents.Controls.GLColumn();
			this.MapLoader = new System.Windows.Forms.ContextMenu();
			this.MenuAtic = new System.Windows.Forms.MenuItem();
			this.ImageListFG = new System.Windows.Forms.ImageList(this.components);
			this.panel1 = new System.Windows.Forms.Panel();
			this.label8 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.ToolbarMain = new System.Windows.Forms.ToolBar();
			this.ButtonLoad = new System.Windows.Forms.ToolBarButton();
			this.ButtonItems = new System.Windows.Forms.ToolBarButton();
			this.ButtonHelp = new System.Windows.Forms.ToolBarButton();
			this.toolBarButton1 = new System.Windows.Forms.ToolBarButton();
			this.ButtonExit = new System.Windows.Forms.ToolBarButton();
			this.panel2 = new System.Windows.Forms.Panel();
			this.RadioXML = new System.Windows.Forms.RadioButton();
			this.RadioDebug = new System.Windows.Forms.RadioButton();
			this.LabelRoomLinks = new System.Windows.Forms.Label();
			this.LabelAttr = new System.Windows.Forms.Label();
			this.LabelID = new System.Windows.Forms.Label();
			this.ListboxRooms = new System.Windows.Forms.ListBox();
			this.LabelRooms = new System.Windows.Forms.Label();
			this.glList = new GlacialComponents.Controls.GlacialList();
			this.label1 = new System.Windows.Forms.Label();
			this.label10 = new System.Windows.Forms.Label();
			this.label13 = new System.Windows.Forms.Label();
			this.ImageListSP = new System.Windows.Forms.ImageList(this.components);
			this.ListBoxBaddies = new System.Windows.Forms.ListBox();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.ListBoxACGKey = new System.Windows.Forms.ListBox();
			this.ListBoxGreen = new System.Windows.Forms.ListBox();
			this.ListBoxRed = new System.Windows.Forms.ListBox();
			this.ListBoxCyan = new System.Windows.Forms.ListBox();
			this.ListBoxYellow = new System.Windows.Forms.ListBox();
			this.panel1.SuspendLayout();
			this.panel2.SuspendLayout();
			this.SuspendLayout();
			// 
			// MapLoader
			// 
			this.MapLoader.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.MenuAtic});
			// 
			// MenuAtic
			// 
			this.MenuAtic.Index = 0;
			this.MenuAtic.Text = "Atic";
			// 
			// ImageListFG
			// 
			this.ImageListFG.ImageSize = new System.Drawing.Size(48, 48);
			this.ImageListFG.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImageListFG.ImageStream")));
			this.ImageListFG.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// panel1
			// 
			this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel1.Controls.Add(this.label8);
			this.panel1.Controls.Add(this.label7);
			this.panel1.Controls.Add(this.label6);
			this.panel1.Controls.Add(this.label5);
			this.panel1.Controls.Add(this.label4);
			this.panel1.Controls.Add(this.ToolbarMain);
			this.panel1.Controls.Add(this.panel2);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
			this.panel1.Location = new System.Drawing.Point(0, 0);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(1016, 54);
			this.panel1.TabIndex = 39;
			// 
			// label8
			// 
			this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label8.Location = new System.Drawing.Point(418, 4);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(231, 16);
			this.label8.TabIndex = 44;
			this.label8.Text = "Keys are randomly placed. one of each.";
			// 
			// label7
			// 
			this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label7.Location = new System.Drawing.Point(846, 26);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(67, 16);
			this.label7.TabIndex = 43;
			this.label7.Text = "Cyan Key";
			// 
			// label6
			// 
			this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label6.Location = new System.Drawing.Point(681, 17);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(143, 28);
			this.label6.TabIndex = 42;
			this.label6.Text = "Red Key (NOTE: mummy is always placed here)";
			// 
			// label5
			// 
			this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label5.Location = new System.Drawing.Point(544, 28);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(67, 16);
			this.label5.TabIndex = 41;
			this.label5.Text = "Green Key";
			// 
			// label4
			// 
			this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label4.Location = new System.Drawing.Point(416, 28);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(67, 16);
			this.label4.TabIndex = 40;
			this.label4.Text = "Yellow Key";
			// 
			// ToolbarMain
			// 
			this.ToolbarMain.Buttons.AddRange(new System.Windows.Forms.ToolBarButton[] {
																						   this.ButtonLoad,
																						   this.ButtonItems,
																						   this.ButtonHelp,
																						   this.toolBarButton1,
																						   this.ButtonExit});
			this.ToolbarMain.Dock = System.Windows.Forms.DockStyle.None;
			this.ToolbarMain.DropDownArrows = true;
			this.ToolbarMain.Location = new System.Drawing.Point(1, -1);
			this.ToolbarMain.Name = "ToolbarMain";
			this.ToolbarMain.ShowToolTips = true;
			this.ToolbarMain.Size = new System.Drawing.Size(211, 42);
			this.ToolbarMain.TabIndex = 2;
			this.ToolbarMain.ButtonClick += new System.Windows.Forms.ToolBarButtonClickEventHandler(this.ToolbarMain_ButtonClick);
			// 
			// ButtonLoad
			// 
			this.ButtonLoad.DropDownMenu = this.MapLoader;
			this.ButtonLoad.Style = System.Windows.Forms.ToolBarButtonStyle.DropDownButton;
			this.ButtonLoad.Tag = "L";
			this.ButtonLoad.Text = "Load Map";
			this.ButtonLoad.ToolTipText = "Loads the map file";
			// 
			// ButtonItems
			// 
			this.ButtonItems.Tag = "O";
			this.ButtonItems.Text = "View Gfx";
			// 
			// ButtonHelp
			// 
			this.ButtonHelp.Tag = "H";
			this.ButtonHelp.Text = "Help";
			// 
			// toolBarButton1
			// 
			this.toolBarButton1.Style = System.Windows.Forms.ToolBarButtonStyle.Separator;
			// 
			// ButtonExit
			// 
			this.ButtonExit.Tag = "X";
			this.ButtonExit.Text = "Exit";
			// 
			// panel2
			// 
			this.panel2.Controls.Add(this.RadioXML);
			this.panel2.Controls.Add(this.RadioDebug);
			this.panel2.Location = new System.Drawing.Point(213, 2);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(86, 44);
			this.panel2.TabIndex = 39;
			// 
			// RadioXML
			// 
			this.RadioXML.Location = new System.Drawing.Point(8, 24);
			this.RadioXML.Name = "RadioXML";
			this.RadioXML.Size = new System.Drawing.Size(61, 16);
			this.RadioXML.TabIndex = 2;
			this.RadioXML.Text = "XML";
			this.RadioXML.Click += new System.EventHandler(this.RadioXML_Click);
			// 
			// RadioDebug
			// 
			this.RadioDebug.Checked = true;
			this.RadioDebug.Location = new System.Drawing.Point(8, 8);
			this.RadioDebug.Name = "RadioDebug";
			this.RadioDebug.Size = new System.Drawing.Size(61, 19);
			this.RadioDebug.TabIndex = 0;
			this.RadioDebug.TabStop = true;
			this.RadioDebug.Text = "Debug";
			this.RadioDebug.Click += new System.EventHandler(this.RadioDebug_Click);
			// 
			// LabelRoomLinks
			// 
			this.LabelRoomLinks.Location = new System.Drawing.Point(238, 73);
			this.LabelRoomLinks.Name = "LabelRoomLinks";
			this.LabelRoomLinks.Size = new System.Drawing.Size(137, 33);
			this.LabelRoomLinks.TabIndex = 22;
			this.LabelRoomLinks.Text = "?";
			// 
			// LabelAttr
			// 
			this.LabelAttr.Location = new System.Drawing.Point(126, 79);
			this.LabelAttr.Name = "LabelAttr";
			this.LabelAttr.Size = new System.Drawing.Size(56, 12);
			this.LabelAttr.TabIndex = 18;
			this.LabelAttr.Text = "?";
			// 
			// LabelID
			// 
			this.LabelID.Location = new System.Drawing.Point(23, 78);
			this.LabelID.Name = "LabelID";
			this.LabelID.Size = new System.Drawing.Size(33, 12);
			this.LabelID.TabIndex = 16;
			this.LabelID.Text = "?";
			// 
			// ListboxRooms
			// 
			this.ListboxRooms.Location = new System.Drawing.Point(9, 118);
			this.ListboxRooms.Name = "ListboxRooms";
			this.ListboxRooms.Size = new System.Drawing.Size(75, 160);
			this.ListboxRooms.TabIndex = 9;
			this.ListboxRooms.SelectedIndexChanged += new System.EventHandler(this.ListboxRooms_SelectedIndexChanged);
			// 
			// LabelRooms
			// 
			this.LabelRooms.Location = new System.Drawing.Point(9, 105);
			this.LabelRooms.Name = "LabelRooms";
			this.LabelRooms.Size = new System.Drawing.Size(100, 11);
			this.LabelRooms.TabIndex = 8;
			this.LabelRooms.Text = "Rooms";
			// 
			// glList
			// 
			this.glList.AllowColumnResize = true;
			this.glList.AllowMultiselect = false;
			this.glList.AlternateBackground = System.Drawing.Color.DarkGreen;
			this.glList.AlternatingColors = false;
			this.glList.AutoHeight = true;
			this.glList.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.glList.BackgroundStretchToFit = true;
			glColumn1.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn1.CheckBoxes = false;
			glColumn1.ImageIndex = -1;
			glColumn1.Name = "TheImage";
			glColumn1.NumericSort = false;
			glColumn1.Text = "Block";
			glColumn1.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn1.Width = 70;
			glColumn2.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn2.CheckBoxes = false;
			glColumn2.ImageIndex = -1;
			glColumn2.Name = "Name";
			glColumn2.NumericSort = false;
			glColumn2.Text = "Name";
			glColumn2.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn2.Width = 230;
			glColumn3.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn3.CheckBoxes = false;
			glColumn3.ImageIndex = -1;
			glColumn3.Name = "x";
			glColumn3.NumericSort = false;
			glColumn3.Text = "x";
			glColumn3.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn3.Width = 35;
			glColumn4.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn4.CheckBoxes = false;
			glColumn4.ImageIndex = -1;
			glColumn4.Name = "y";
			glColumn4.NumericSort = false;
			glColumn4.Text = "y";
			glColumn4.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn4.Width = 35;
			glColumn5.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn5.CheckBoxes = false;
			glColumn5.ImageIndex = -1;
			glColumn5.Name = "extra";
			glColumn5.NumericSort = false;
			glColumn5.Text = "Flags/Type";
			glColumn5.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn5.Width = 70;
			glColumn6.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn6.CheckBoxes = false;
			glColumn6.ImageIndex = -1;
			glColumn6.Name = "doortimer";
			glColumn6.NumericSort = false;
			glColumn6.Text = "Door Timer";
			glColumn6.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn6.Width = 80;
			glColumn7.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn7.CheckBoxes = false;
			glColumn7.ImageIndex = -1;
			glColumn7.Name = "otherdoor";
			glColumn7.NumericSort = false;
			glColumn7.Text = "Door Exit Flag";
			glColumn7.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn7.Width = 80;
			glColumn8.ActivatedEmbeddedType = GlacialComponents.Controls.GLActivatedEmbeddedTypes.None;
			glColumn8.CheckBoxes = false;
			glColumn8.ImageIndex = -1;
			glColumn8.Name = "Description";
			glColumn8.NumericSort = false;
			glColumn8.Text = "Description";
			glColumn8.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
			glColumn8.Width = 340;
			this.glList.Columns.AddRange(new GlacialComponents.Controls.GLColumn[] {
																					   glColumn1,
																					   glColumn2,
																					   glColumn3,
																					   glColumn4,
																					   glColumn5,
																					   glColumn6,
																					   glColumn7,
																					   glColumn8});
			this.glList.ControlStyle = GlacialComponents.Controls.GLControlStyles.Normal;
			this.glList.FullRowSelect = true;
			this.glList.GridColor = System.Drawing.Color.LightGray;
			this.glList.GridLines = GlacialComponents.Controls.GLGridLines.gridBoth;
			this.glList.GridLineStyle = GlacialComponents.Controls.GLGridLineStyles.gridSolid;
			this.glList.GridTypes = GlacialComponents.Controls.GLGridTypes.gridOnExists;
			this.glList.HeaderHeight = 22;
			this.glList.HeaderVisible = true;
			this.glList.HeaderWordWrap = false;
			this.glList.HotColumnTracking = false;
			this.glList.HotItemTracking = false;
			this.glList.HotTrackingColor = System.Drawing.Color.LightGray;
			this.glList.HoverEvents = false;
			this.glList.HoverTime = 1;
			this.glList.ImageList = this.ImageListFG;
			this.glList.ItemHeight = 17;
			this.glList.ItemWordWrap = false;
			this.glList.Location = new System.Drawing.Point(89, 112);
			this.glList.Name = "glList";
			this.glList.Selectable = true;
			this.glList.SelectedTextColor = System.Drawing.Color.White;
			this.glList.SelectionColor = System.Drawing.Color.DarkBlue;
			this.glList.ShowBorder = true;
			this.glList.ShowFocusRect = false;
			this.glList.Size = new System.Drawing.Size(918, 531);
			this.glList.SortType = GlacialComponents.Controls.SortTypes.InsertionSort;
			this.glList.SuperFlatHeaderColor = System.Drawing.Color.White;
			this.glList.TabIndex = 45;
			this.glList.Text = "glacialList1";
			// 
			// label1
			// 
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label1.Location = new System.Drawing.Point(92, 62);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(120, 16);
			this.label1.TabIndex = 13;
			this.label1.Text = "Attributes (colour)";
			// 
			// label10
			// 
			this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label10.Location = new System.Drawing.Point(15, 61);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(67, 16);
			this.label10.TabIndex = 10;
			this.label10.Text = "ID/Location";
			// 
			// label13
			// 
			this.label13.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label13.Location = new System.Drawing.Point(215, 62);
			this.label13.Name = "label13";
			this.label13.Size = new System.Drawing.Size(147, 16);
			this.label13.TabIndex = 11;
			this.label13.Text = "Rooms Linked From Here";
			// 
			// ImageListSP
			// 
			this.ImageListSP.ImageSize = new System.Drawing.Size(32, 32);
			this.ImageListSP.TransparentColor = System.Drawing.Color.Transparent;
			// 
			// ListBoxBaddies
			// 
			this.ListBoxBaddies.Location = new System.Drawing.Point(10, 315);
			this.ListBoxBaddies.Name = "ListBoxBaddies";
			this.ListBoxBaddies.Size = new System.Drawing.Size(75, 134);
			this.ListBoxBaddies.TabIndex = 47;
			this.ListBoxBaddies.SelectedIndexChanged += new System.EventHandler(this.ListBoxBaddies_SelectedIndexChanged);
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(10, 285);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(65, 27);
			this.label2.TabIndex = 46;
			this.label2.Text = "Baddies/ Baddy Killers";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(11, 457);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(65, 72);
			this.label3.TabIndex = 51;
			this.label3.Text = "ACG Key Parts (8 rnd locations - but always 3 together)";
			// 
			// ListBoxACGKey
			// 
			this.ListBoxACGKey.Location = new System.Drawing.Point(8, 529);
			this.ListBoxACGKey.Name = "ListBoxACGKey";
			this.ListBoxACGKey.Size = new System.Drawing.Size(75, 108);
			this.ListBoxACGKey.TabIndex = 50;
			// 
			// ListBoxGreen
			// 
			this.ListBoxGreen.ColumnWidth = 30;
			this.ListBoxGreen.Location = new System.Drawing.Point(510, 64);
			this.ListBoxGreen.MultiColumn = true;
			this.ListBoxGreen.Name = "ListBoxGreen";
			this.ListBoxGreen.Size = new System.Drawing.Size(160, 43);
			this.ListBoxGreen.TabIndex = 54;
			this.ListBoxGreen.SelectedIndexChanged += new System.EventHandler(this.ListBoxGreen_SelectedIndexChanged);
			// 
			// ListBoxRed
			// 
			this.ListBoxRed.ColumnWidth = 30;
			this.ListBoxRed.Location = new System.Drawing.Point(678, 64);
			this.ListBoxRed.MultiColumn = true;
			this.ListBoxRed.Name = "ListBoxRed";
			this.ListBoxRed.Size = new System.Drawing.Size(160, 43);
			this.ListBoxRed.TabIndex = 55;
			this.ListBoxRed.SelectedIndexChanged += new System.EventHandler(this.ListBoxRed_SelectedIndexChanged);
			// 
			// ListBoxCyan
			// 
			this.ListBoxCyan.ColumnWidth = 30;
			this.ListBoxCyan.Location = new System.Drawing.Point(844, 62);
			this.ListBoxCyan.MultiColumn = true;
			this.ListBoxCyan.Name = "ListBoxCyan";
			this.ListBoxCyan.Size = new System.Drawing.Size(160, 43);
			this.ListBoxCyan.TabIndex = 56;
			this.ListBoxCyan.SelectedIndexChanged += new System.EventHandler(this.ListBoxCyan_SelectedIndexChanged);
			// 
			// ListBoxYellow
			// 
			this.ListBoxYellow.ColumnWidth = 3;
			this.ListBoxYellow.Location = new System.Drawing.Point(392, 65);
			this.ListBoxYellow.Name = "ListBoxYellow";
			this.ListBoxYellow.Size = new System.Drawing.Size(113, 43);
			this.ListBoxYellow.TabIndex = 57;
			this.ListBoxYellow.SelectedIndexChanged += new System.EventHandler(this.ListBoxYellow_SelectedIndexChanged);
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(1016, 711);
			this.Controls.Add(this.ListBoxYellow);
			this.Controls.Add(this.ListBoxCyan);
			this.Controls.Add(this.ListBoxRed);
			this.Controls.Add(this.ListBoxGreen);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.ListBoxACGKey);
			this.Controls.Add(this.ListBoxBaddies);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.glList);
			this.Controls.Add(this.panel1);
			this.Controls.Add(this.LabelRoomLinks);
			this.Controls.Add(this.LabelAttr);
			this.Controls.Add(this.LabelID);
			this.Controls.Add(this.ListboxRooms);
			this.Controls.Add(this.LabelRooms);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.label10);
			this.Controls.Add(this.label13);
			this.Name = "MainForm";
			this.Text = "Ultimate Atic Atac Viewer";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
			this.panel1.ResumeLayout(false);
			this.panel2.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new MainForm());
		}

		private void ListboxRooms_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			RoomChange();
		}

		private void StartUpRoom()
		{
			myData.LoadData("maps\\aa_map.txt","aa");
			//convert loaded data into internal format
			if(!myData.TransformData()) MessageBox.Show(this,"Errors occurred in the data loading.  Consult the debug or raw output files (search for ERROR)");
			myData.XMLOutput("Atic Atac");
			UpdateRoomDisplay();

			//set special listboxes
			this.ListBoxBaddies.Items.Clear();
			this.ListBoxACGKey.Items.Clear();
			this.ListBoxCyan.Items.Clear();
			this.ListBoxYellow.Items.Clear();
			this.ListBoxRed.Items.Clear();
			this.ListBoxGreen.Items.Clear();
			foreach(PlacedObject po in myData.PlacedObjects)
			{
				if(po.Type==3 || po.Type==4 || po.Type==5)
					this.ListBoxBaddies.Items.Add(po.Room);
			}
			for(int j=0;j<myData.ACGKeyData.Count-1;j+=3)
			{
				this.ListBoxACGKey.Items.Add(myData.ACGKeyData[j] + ", " +myData.ACGKeyData[j+1] + ", " +myData.ACGKeyData[j+2]);
			}
			foreach(int i in myData.RedKeyData)
				this.ListBoxRed.Items.Add(i);
			foreach(int i in myData.CyanKeyData)
				this.ListBoxCyan.Items.Add(i);
			foreach(int i in myData.GreenKeyData)
				this.ListBoxGreen.Items.Add(i);
			this.ListBoxYellow.Items.Add(myData.YellowKey);

			MessageBox.Show(this,"Generated data files are stored in the 'outputs' directory - xml and csv files");
		}

		private void RoomChange()
		{
			Hashtable hl;
			hl=new Hashtable();
			this.glList.Items.Clear();
			this.ImageListFG.Images.Clear();
			Room lRoom=(Room)myData.Rooms[this.ListboxRooms.SelectedIndex];

			this.LabelRoomLinks.Text="";
			foreach (Block tmpBlock in lRoom.RoomLinkItems)
				this.LabelRoomLinks.Text+=(tmpBlock.RoomLink+", ");

			this.LabelID.Text=lRoom.ID.ToString();
			this.LabelAttr.Text=lRoom.Attribute.ToString();
			//background
			BlockDescriptor bd;
			int locationx,locationy;
			string flags;
			int type;
			string desc;
			//background as there are just a few simply add the bitmaps
			//without checking for duplicates.

			//add room
			GlacialComponents.Controls.GLItem gli1=new GlacialComponents.Controls.GLItem();
			GlacialComponents.Controls.GLSubItem gls1;
			try 
			{
				if(myData.FindRoomTypeFromNumber(lRoom.Type).bmp!="")
					this.ImageListFG.Images.Add(new Bitmap("images\\"+myData.FindRoomTypeFromNumber(lRoom.Type).bmp));
			} 
			catch 
			{
				this.ImageListFG.Images.Add(new Bitmap("images\\unknown.gif"));
				System.Windows.Forms.MessageBox.Show(this,"Could not open bitmap: " + "images\\"+myData.FindRoomTypeFromNumber(lRoom.Type).bmp);
			}
			gls1=new GlacialComponents.Controls.GLSubItem();
			gls1.ImageIndex=(this.ImageListFG.Images.Count-1);
			gli1.SubItems.Add(gls1);

			gls1=new GlacialComponents.Controls.GLSubItem();
			gls1.Text="Room Type "+lRoom.Type + "("+myData.FindRoomTypeFromNumber(lRoom.Type).Name+")"; //name
			gli1.SubItems.Add(gls1);
			gli1.BackColor=System.Drawing.Color.Chartreuse;
			glList.Items.Add(gli1);
			
			foreach(Block block in lRoom.RoomLinkItems)
			{
				bd=myData.FindBlockFromNumber(block.Number);
				try 
				{
					if(bd.bmp!="")
						this.ImageListFG.Images.Add(new Bitmap("images\\"+bd.bmp));
				} 
				catch 
				{
					this.ImageListFG.Images.Add(new Bitmap("images\\unknown.gif"));
					System.Windows.Forms.MessageBox.Show(this,"Could not open bitmap: " + "images\\"+bd.bmp);
				}
				locationx=block.x;
				locationy=block.y;
				flags=block.Flags;

				GlacialComponents.Controls.GLItem gli=new GlacialComponents.Controls.GLItem();
				GlacialComponents.Controls.GLSubItem gls;
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.ImageIndex=(this.ImageListFG.Images.Count-1);
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=bd.Name+" To Room "+block.RoomLink.ToString(); //name
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=locationx.ToString(); //x
				gli.SubItems.Add(gls);
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=locationy.ToString(); //y
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=flags.ToString(); //y
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=block.DoorTime.ToString(); //y
				gli.SubItems.Add(gls);
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=block.DoorFlag.ToString(); //y
				gli.SubItems.Add(gls);

				gli.BackColor=System.Drawing.Color.Bisque;
				glList.Items.Add(gli);
			}

			foreach(Block block in lRoom.StaticItems)
			{
				bd=myData.FindBlockFromNumber(block.Number);
				try 
				{
					if(bd.bmp!="")
						this.ImageListFG.Images.Add(new Bitmap("images\\"+bd.bmp));
				} 
				catch 
				{
					this.ImageListFG.Images.Add(new Bitmap("images\\unknown.gif"));
					System.Windows.Forms.MessageBox.Show(this,"Could not open bitmap: " + "images\\"+bd.bmp);
				}
				locationx=block.x;
				locationy=block.y;
				GlacialComponents.Controls.GLItem gli=new GlacialComponents.Controls.GLItem();
				GlacialComponents.Controls.GLSubItem gls;
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.ImageIndex=(this.ImageListFG.Images.Count-1);
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=bd.Name;
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=locationx.ToString(); //x
				gli.SubItems.Add(gls);
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=locationy.ToString(); //y
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=block.Flags.ToString(); //y
				gli.SubItems.Add(gls);

				//gli.BackColor=System.Drawing.Color.Bisque;
				glList.Items.Add(gli);
			}

			//placed items
			foreach(PlacedObject po in lRoom.PlacedItems)
			{
				try 
				{
					if(po.bmp!="")
						this.ImageListFG.Images.Add(new Bitmap("images\\"+po.bmp));
				} 
				catch 
				{
					this.ImageListFG.Images.Add(new Bitmap("images\\unknown.gif"));
					System.Windows.Forms.MessageBox.Show(this,"Could not open bitmap: " + "images\\"+po.bmp);
				}
				locationx=po.x;
				locationy=po.y;
				type=po.Type;
				desc=po.desc;

				GlacialComponents.Controls.GLItem gli=new GlacialComponents.Controls.GLItem();
				GlacialComponents.Controls.GLSubItem gls;
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.ImageIndex=(this.ImageListFG.Images.Count-1);
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=po.Name;
				gli.SubItems.Add(gls);

				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=locationx.ToString(); //x
				gli.SubItems.Add(gls);
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=locationy.ToString(); //y
				gli.SubItems.Add(gls);
				
				string sType;
				gls=new GlacialComponents.Controls.GLSubItem();
				switch(type)
				{
					case 1:
						sType="Food";
						break;
					case 2:
						sType="Energy Sap";
						break;
					case 3:
						sType="Kills Baddie";
						break;
					case 4:
						sType="Baddie";
						break;
					case 5:
						sType="Baddie Roaming";
						break;
					case 6:
						sType="Special?";
						break;
					default:
						sType="Error!";
						break;
				}
				gls.Text=sType; //type
				gli.SubItems.Add(gls);
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=""; //ignore
				gli.SubItems.Add(gls);
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=""; //ignore
				gli.SubItems.Add(gls);
				gls=new GlacialComponents.Controls.GLSubItem();
				gls.Text=desc; //desc
				gli.SubItems.Add(gls);

				//gli.BackColor=System.Drawing.Color.Bisque;
				glList.Items.Add(gli);
			}

			int offsetimages=this.ImageListFG.Images.Count;
			hl.Clear();

			if(hl.Count!=this.ImageListFG.Images.Count-offsetimages) System.Windows.Forms.MessageBox.Show(this,"Not all foreground images could be found for display in the treeview");
			//objects
			glList.Refresh();
		}


		private void ToolbarMain_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e)
		{
			switch((string)e.Button.Tag)
			{
				case "L":
					MessageBox.Show(this,"Select the drop down to choose your map.");
					break;
				case "H":
					SplashScreen splashscreen=new SplashScreen(true);
					splashscreen.ShowDialog(this);
					break;
				case "X":
					this.Close();
					Application.Exit();
					break;
				case "O":
					images imagescreen=new images();
					imagescreen.DoList(myData);
					imagescreen.ShowDialog(this);
					break;
				default:
					MessageBox.Show(this,"You have selected a menu not yet defined!");
					break;
			}
		}

		private void UpdateRoomDisplay()
		{
			this.ListboxRooms.Items.Clear();
			this.LabelRooms.Text="Rooms: " +myData.Rooms.Count;
			foreach(Room eachRoom in myData.Rooms)
				this.ListboxRooms.Items.Add("Room " +eachRoom.ID);

			if(this.ListboxRooms.Items.Count>0)
				this.ListboxRooms.SelectedIndex=0;

		}

		private bool DisplayOutputToText(string filetoload)
		{
			outputwindow mywindow=new outputwindow();
			if(mywindow.LoadData(filetoload))
				mywindow.ShowDialog(this);

			mywindow.Close();
			return true;
		}

		private void RadioDebug_Click(object sender, System.EventArgs e)
		{
			DisplayOutputToText("outputs\\"+myData.Prefixname+"_debuglog.txt");
		}

		private void RadioRaw_Click(object sender, System.EventArgs e)
		{
			DisplayOutputToText("outputs\\"+myData.Prefixname+"_rawstructure.txt");
		}

		private void RadioXML_Click(object sender, System.EventArgs e)
		{
			MessageBox.Show(this,"Use a browser or other application to view the XML file using the pre-defined XML template or the pre-defined XML schema.");
			DisplayOutputToText("outputs\\"+myData.Prefixname+"_xml.xml");
		}

		private void ListBoxBaddies_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			int iIndex= this.ListboxRooms.FindString("Room " +this.ListBoxBaddies.Text);

			this.ListboxRooms.SelectedIndex=iIndex;
		}

		private void ListBoxYellow_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			this.ListboxRooms.SelectedIndex=this.ListboxRooms.FindString("Room " +this.ListBoxYellow.SelectedItem.ToString());
		}

		private void ListBoxGreen_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			this.ListboxRooms.SelectedIndex=this.ListboxRooms.FindString("Room " +this.ListBoxGreen.SelectedItem.ToString());
		}

		private void ListBoxRed_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			this.ListboxRooms.SelectedIndex=this.ListboxRooms.FindString("Room " +this.ListBoxRed.SelectedItem.ToString());
		}

		private void ListBoxCyan_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			this.ListboxRooms.SelectedIndex=this.ListboxRooms.FindString("Room " +this.ListBoxCyan.SelectedItem.ToString());
		}

	}
}
