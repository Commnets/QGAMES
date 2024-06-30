using System;
using System.Collections;
using System.IO;
using System.Text;

namespace Retrospec.IsoConverter
{
	/// <summary>
	/// Converts Ultimate Isometric code
	/// </summary>

	public class IsoData
	{
		public ArrayList Rooms=new ArrayList();
		private StreamWriter debug=null;
		public ArrayList LocationData=new ArrayList();
		public ArrayList ObjectData=new ArrayList();
		public ArrayList DefinedBlocks=new ArrayList();
		public ArrayList DefinedRooms=new ArrayList();
		public ArrayList PlacedObjects=new ArrayList();
		public ArrayList ACGKeyData=new ArrayList();
		public ArrayList RedKeyData=new ArrayList();	//this also places the mummy
		public ArrayList CyanKeyData=new ArrayList();
		public ArrayList GreenKeyData=new ArrayList(); 
		public int YellowKey;		//only one location
		public string Filename;
		public string Prefixname;
		public int StartRoom;
		public BlockDescriptor FindBlockFromNumber(int numberToFind)
		{
			foreach(BlockDescriptor mybd in DefinedBlocks)
				if(mybd.Number==numberToFind) return mybd;

			return (BlockDescriptor)DefinedBlocks[0];
		}
		public BlockDescriptor FindRoomTypeFromNumber(int numberToFind)
		{
			foreach(BlockDescriptor mybd in DefinedRooms)
				if(mybd.Number==numberToFind) return mybd;

			return (BlockDescriptor)DefinedBlocks[0];
		}
		public BlockDescriptor FindBlockFromString(int btype,string NameToFind)
		{
			foreach(BlockDescriptor mybd in DefinedBlocks)
				if(mybd.Name==NameToFind) return mybd;

			return (BlockDescriptor)DefinedBlocks[0];
		}

		public void LoadData(string file,string prefixname)
		{
			LoadData(file,prefixname,"");
		}
		
		public void LoadData(string file,string prefixname,string additionalLocation)
		{
			//populate data
			Filename=file;
			Prefixname=prefixname;
			LocationData.Clear();
			ObjectData.Clear();
			DefinedBlocks.Clear();
			DefinedRooms.Clear();
			PlacedObjects.Clear();
			ACGKeyData.Clear();

			char[] bob2=",".ToCharArray();
			string lineread;
			StreamReader sr;

			//normal
			sr=new StreamReader(this.Filename);
			//first line - remarks
			//second line - locations
			sr.ReadLine();
			lineread=sr.ReadLine();
			string[] LocationDataString=lineread.Split(bob2);
			foreach(string element in LocationDataString)
				LocationData.Add(Convert.ToInt32(element,16));

			//third,fourth,fifth lines - comments
			//sixth line=objectdata - split later
			sr.ReadLine();
			sr.ReadLine();
			sr.ReadLine();
			lineread=sr.ReadLine();
			string[] ObjectDataString=lineread.Split(bob2);
			foreach(string element in ObjectDataString)
				ObjectData.Add(Convert.ToInt32(element,16));

			//seventh line - start room
			lineread=sr.ReadLine();
			string[] Starting=lineread.Split(bob2);
			StartRoom=Convert.ToInt32(Starting[0],16);

			//line 8. acg key
			sr.ReadLine();
			lineread=sr.ReadLine();
			string[] ACGKeyDataString=lineread.Split(bob2);
			foreach(string element in ACGKeyDataString)
				ACGKeyData.Add(Convert.ToInt32(element,16));
			//yellokey
			sr.ReadLine();
			lineread=sr.ReadLine();
			YellowKey=Convert.ToInt32(lineread,16);
			//red key			
			sr.ReadLine();
			lineread=sr.ReadLine();
			string[] RedKeyDataString=lineread.Split(bob2);
			foreach(string element in RedKeyDataString)
				RedKeyData.Add(Convert.ToInt32(element,16));
			//cyan key			
			sr.ReadLine();
			lineread=sr.ReadLine();
			string[] CyanKeyDataString=lineread.Split(bob2);
			foreach(string element in CyanKeyDataString)
				CyanKeyData.Add(Convert.ToInt32(element,16));
			//green key			
			sr.ReadLine();
			lineread=sr.ReadLine();
			string[] GreenKeyDataString=lineread.Split(bob2);
			foreach(string element in GreenKeyDataString)
				GreenKeyData.Add(Convert.ToInt32(element,16));

			//line x - ignore - start of data
			sr.ReadLine();

			//9 onwards, rooms
			bool MoreData=true;
			BlockDescriptor BDLine=new BlockDescriptor();
			//add not found element
			BDLine.Number=-1;
			BDLine.Name="**unknown seek help**";
			DefinedRooms.Add(BDLine);
			DebugOut("Doing Room Types");
			do
			{
				lineread=sr.ReadLine();
				if(lineread.StartsWith("-")) MoreData=false;
				else
				{
					BDLine=new BlockDescriptor();
					string[] BlockDataString=lineread.Split(bob2);
					BDLine.Number=Convert.ToInt32(BlockDataString[0],16);
					BDLine.Name=BlockDataString[1];
					BDLine.bmp=BlockDataString[2];
					DefinedRooms.Add(BDLine);
					DebugOut(BDLine.Number+"\t"+BDLine.bmp+"\t"+BDLine.Name);
				}
			} while(MoreData);
			DebugOut("Found "+(DefinedRooms.Count-1).ToString()+" room types");
			DebugOut("-----------------------------------------");
			//next line is also a comment
			sr.ReadLine();

			//after rooms, room blocks
			MoreData=true;
			BDLine=new BlockDescriptor();
			//add not found element
			BDLine.Number=-1;
			BDLine.Name="**unknown seek help**";
			DefinedBlocks.Add(BDLine);
			DebugOut("Doing Room Objects");
			do
			{
				lineread=sr.ReadLine();
				if(lineread.StartsWith("-")) MoreData=false;
				else
				{
					BDLine=new BlockDescriptor();
					string[] BlockDataString=lineread.Split(bob2);
					BDLine.Number=Convert.ToInt32(BlockDataString[0],16);
					BDLine.Name=BlockDataString[1];
					BDLine.bmp=BlockDataString[2];
					BDLine.Type=Convert.ToInt32(BlockDataString[3],10);
					if(BlockDataString[3]=="1") 
						BDLine.IgnoreDoor=true;
					else 
						BDLine.IgnoreDoor=false;
					DefinedBlocks.Add(BDLine);
					DebugOut(BDLine.Number+"\t"+BDLine.bmp+"\t"+BDLine.Name+" type: "+BDLine.Type+"\tDoor Ignore: "+BDLine.IgnoreDoor);
				}
			} while(MoreData);
			DebugOut("Found "+(DefinedBlocks.Count-1).ToString()+" block types");
			DebugOut("-----------------------------------------");

			//after blocks, placed objects
			//three comment lines
			sr.ReadLine();
			sr.ReadLine();
			sr.ReadLine();

			MoreData=true;
			PlacedObject POCurrent;
			DebugOut("Doing Placed Objects");
			int ipCount=1001;	//do not interfere with other objects
			do
			{
				lineread=sr.ReadLine();
				if(lineread.StartsWith("-")) MoreData=false;
				else
				{
					POCurrent=new PlacedObject();
					string[] BlockDataString=lineread.Split(bob2);
					POCurrent.Number=1000+Convert.ToInt32(BlockDataString[0],16);
					POCurrent.Room=Convert.ToInt32(BlockDataString[1],16);
					POCurrent.x=Convert.ToInt32(BlockDataString[2],16);
					POCurrent.y=Convert.ToInt32(BlockDataString[3],16);
					POCurrent.Name=BlockDataString[4];
					POCurrent.bmp=BlockDataString[5];
					POCurrent.Type=Convert.ToInt32(BlockDataString[6],16);
					POCurrent.desc=BlockDataString[7];
					POCurrent.SequenceCount=ipCount;
					ipCount++;
					this.PlacedObjects.Add(POCurrent);
					DebugOut(POCurrent.Number+"\t"+POCurrent.bmp+"\t"+POCurrent.Name+"\ttype:"+POCurrent.Type);
				}
			} while(MoreData);
			DebugOut("Found "+(PlacedObjects.Count-1).ToString()+" placed objects");
			DebugOut("-----------------------------------------");

			sr.Close();
		}

		public int FindRoomIndex(int roomnumber)
		{
			for(int i=0;i<Rooms.Count;i++)
				if(((Room)Rooms[i]).ID==roomnumber) return i;

			return -1;
		}

		public bool TransformData()
		{
			//convert location/object data to our internal format

			//location data
			//byte 0: room number
			//byte 1: attribute
			bool bRet=true;
			int CurrentItem=0;
			Room CurrentRoom=null;  //initialised below

			//loop until end of data
			Rooms.Clear();
			DebugOut("");
			DebugOut("Doing Rooms");
			DebugOut("");
			int i=0;
			while(CurrentItem<LocationData.Count)
			{
				//create room
				CurrentRoom=new Room();
				CurrentRoom.ID=i;
				i++;
				//byte 0
				CurrentRoom.Attribute=(int)LocationData[CurrentItem];
				//byte 1
				CurrentItem++;
				CurrentRoom.Type=(int)LocationData[CurrentItem];
				Rooms.Add(CurrentRoom);
				CurrentItem++;
				DebugOut("Room: "+CurrentRoom.ID+" \tType: "+CurrentRoom.Type+" \tColour: "+CurrentRoom.Attribute);
			}

			//object data
			CurrentItem=0;
			int type,room,x,y,doortime,doorflag,iflags,iCount,bIgnoreDoor;
			DebugOut("");
			DebugOut("-------------------------------------------------");
			DebugOut("Doing Room Objects");
			DebugOut("");
			iCount=0;
			for(CurrentItem=0;CurrentItem<ObjectData.Count;CurrentItem+=7)
			{
				type=(int)ObjectData[CurrentItem];
				room=(int)ObjectData[CurrentItem+1];
				x=(int)ObjectData[CurrentItem+2];
				y=(int)ObjectData[CurrentItem+3];
				iflags=(int)ObjectData[CurrentItem+4];
				doortime=(int)ObjectData[CurrentItem+5];
				doorflag=(int)ObjectData[CurrentItem+6];

				Block tBlock=new Block();
				tBlock.SequenceCount=iCount;
				iCount++;
				tBlock.Number=type;
				tBlock.x=x;
				tBlock.y=y;
				tBlock.iFlags=iflags;
				tBlock.DoorFlag=doorflag;
				tBlock.DoorTime=doortime;
				switch(iflags)
				{
					case 0:
						tBlock.Flags="Top";
						break;
					case 0x60:
						tBlock.Flags="Right";
						break;
					case 0x80:
						tBlock.Flags="Bottom";
						break;
					case 0xe0:
						tBlock.Flags="Left";
						break;
					case 3:
					case 4:
					case 0x41:
					case 1:
					case 0x61:
					case 0x81:
					case 0xe1:
						tBlock.Flags=iflags.ToString();
						break;
					default:
						tBlock.Flags="unknown!";
						break;
				}


				BlockDescriptor tmp;
				try
				{
					if(doorflag!=0)
					{
						tmp=this.FindBlockFromNumber(type);
						//populate link
						if(tmp.IgnoreDoor)
						{
							((Room)Rooms[this.FindRoomIndex(room)]).StaticItems.Add(tBlock);
							DebugOut(" Room: " + room + ",\tStatic Item (changed from room link)  : "+type+"\t("+tmp.Name+"), x,y: "+x+","+y+" \tflags,doortime,doorflag: "+iflags+", "+doortime+", "+doorflag);
						}
						else
						{
							if((CurrentItem%2)==0 || CurrentItem==0)
								tBlock.RoomLink=(int)ObjectData[CurrentItem+8];
							else
								tBlock.RoomLink=(int)ObjectData[CurrentItem-6];
							((Room)Rooms[this.FindRoomIndex(room)]).RoomLinkItems.Add(tBlock);
							DebugOut(" Room: " + room + ",\tRoom Link Item: "+type+"\t("+tmp.Name+"), x,y: "+x+","+y+" \tflags,doortime,doorflag: "+iflags+", "+doortime+", "+doorflag);
						}
					}
					else
					{
						tmp=this.FindBlockFromNumber(type);
						((Room)Rooms[this.FindRoomIndex(room)]).StaticItems.Add(tBlock);
						DebugOut(" Room: " + room + ",\tStatic Item  : "+type+"\t("+tmp.Name+"), x,y: "+x+","+y+" \tflags,doortime,doorflag: "+iflags+", "+doortime+", "+doorflag);
					}
				}
				catch
				{
					DebugOut(" Room ERROR: does not exist: " + room);
					bRet=false;
				}
			}

			int roomindex;
			for(int j=0;j<PlacedObjects.Count;j++)
			{
				roomindex=this.FindRoomIndex(((PlacedObject)PlacedObjects[j]).Room);
				((Room)Rooms[roomindex]).PlacedItems.Add(PlacedObjects[j]);
			}

			//debug room links
			DebugOut("");
			DebugOut("-------------------------------------------------");
			DebugOut("Showing Room Links");
			DebugOut("");
			string tmps;
			foreach(Room room1 in Rooms)
			{
				tmps="";
				foreach(Block block in room1.RoomLinkItems)
					tmps+=(block.RoomLink+", ");

				DebugOut("Room: "+room1.ID+", Links: "+tmps);
			}

			if(this.debug!=null)
			{
				debug.Flush();
				debug.Close();
				debug=null;
			}
			return bRet;
		}

		public void DebugOut(string display)
		{
			if(debug==null)
				debug = new StreamWriter("outputs\\"+this.Prefixname+"_debuglog.txt");
			debug.WriteLine(display);
		}

		public string XMLOutput(string nicename)
		{
			StreamWriter sw;
			try 
			{
				sw=new StreamWriter("outputs\\"+this.Prefixname+"_xml.xml");
			}
			catch 
			{
				return "Failed to create output file outputs\\"+this.Prefixname+"_xml.xml";
			}
			try 
			{
				//header information
				sw.WriteLine("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
				sw.WriteLine("<map id=\"Atic Atac\" >");
				sw.WriteLine("\t<!-- just general comments -->");
				sw.WriteLine("\t<comment desc=\"wherever red key is placed, that is where the mummy - id of 21 - is placed\" />");
				sw.WriteLine("\t<comment desc=\"when player is knight he moves through clocks only\" />");
				sw.WriteLine("\t<comment desc=\"when player is wizard he moves through bookcase only\" />");
				sw.WriteLine("\t<comment desc=\"when player is surf he moves through barrels onlyh\" />");
				sw.WriteLine("\t<!-- where to place the acg key. random of 8 set positions -->");
				sw.WriteLine("\t<acg_keylocations>");

				//acg keys
				for(int j=0;j<ACGKeyData.Count-1;j+=3)
					sw.WriteLine("\t\t<acg_keylocation left=\"" + ACGKeyData[j]+"\" middle=\""+ACGKeyData[j+1]+"\" right=\""+ACGKeyData[j+2]+"\" />");
				sw.WriteLine("\t</acg_keylocations>");

				//keys
				sw.WriteLine("\t<!-- reg/green/cyan/yellow keys. pick one at random from each line -->");
				sw.WriteLine("\t<yellowkey room1=\""+this.YellowKey+"\" />");
				sw.WriteLine("\t<redkey room1=\""+this.RedKeyData[0]+"\" room2=\""+this.RedKeyData[1]+"\" room3=\""+this.RedKeyData[2]+"\" room4=\""+this.RedKeyData[3]+"\" room5=\""+this.RedKeyData[4]+"\" room6=\""+this.RedKeyData[5]+"\" room7=\""+this.RedKeyData[6]+"\" room8=\""+this.RedKeyData[7]+"\" />");
				sw.WriteLine("\t<cyankey room1=\""+this.CyanKeyData[0]+"\" room2=\""+this.CyanKeyData[1]+"\" room3=\""+this.CyanKeyData[2]+"\" room4=\""+this.CyanKeyData[3]+"\" room5=\""+this.CyanKeyData[4]+"\" room6=\""+this.CyanKeyData[5]+"\" room7=\""+this.CyanKeyData[6]+"\" room8=\""+this.CyanKeyData[7]+"\" />");
				sw.WriteLine("\t<greenkey room1=\""+this.GreenKeyData[0]+"\" room2=\""+this.GreenKeyData[1]+"\" room3=\""+this.GreenKeyData[2]+"\" room4=\""+this.GreenKeyData[3]+"\" room5=\""+this.GreenKeyData[4]+"\" room6=\""+this.GreenKeyData[5]+"\" room7=\""+this.GreenKeyData[6]+"\" room8=\""+this.GreenKeyData[7]+"\" />");
				//rooms
				sw.WriteLine("");
				sw.WriteLine("\t<!-- rooms -->");	
				sw.WriteLine("\t<rooms startroom=\""+this.StartRoom.ToString()+"\""+ " >");
				sw.WriteLine("\t\t<!--");
				sw.WriteLine("\t\t\tid is the room number");
				sw.WriteLine("\t\t\ttype is the room type - use lookup_roomtypes to find the shape of it");
				sw.WriteLine("\t\t-->");
				foreach(Room lRoom in this.Rooms)
					sw.WriteLine("\t\t<room id=\""+lRoom.ID.ToString()+"\" type=\""+lRoom.Type.ToString()+"\" />");
					//sw.WriteLine("\t\t<room id=\""+lRoom.ID.ToString()+"\" type \""+lRoom.Type.ToString()+"\" attribute=\""+lRoom.Attribute.ToString()+"\" />");
				sw.WriteLine("\t</rooms>");
				sw.WriteLine("\t<roomobjects>");
				sw.WriteLine("\t\t<!--");
				sw.WriteLine("\t\t\tid is used to link jump items together");
				sw.WriteLine("\t\t\titem is the object number to find in the lookup_roomobjects table and determines what the object does");
				sw.WriteLine("\t\t\troom is the room it is in");
				sw.WriteLine("\t\t\tflags is usually the location of the object");
				sw.WriteLine("\t\t\tdoorflags is probably not used");
				sw.WriteLine("\t\t\tjumpto_room is the room to jump to when go through this item");
				sw.WriteLine("\t\t\tjumpto_object is the item to come out of when jumpto_room is invoked");
				sw.WriteLine("\t\t\tx/y is the position");
				sw.WriteLine("\t\t-->");
				foreach(Room lRoom in this.Rooms)
				{
					//start from 0
					//static items start from 100
					//placed items start from 200

					//objects that are links
					foreach(Block myBlock in lRoom.RoomLinkItems)
					{
						int nObjJump=-1;
						if(myBlock.SequenceCount==0 || (myBlock.SequenceCount % 2 ==0)) nObjJump=myBlock.SequenceCount+1;
						else nObjJump=myBlock.SequenceCount-1;

						sw.WriteLine("\t\t<roomobject id=\""+myBlock.SequenceCount.ToString()+"\" item=\""+myBlock.Number+"\" room=\""+lRoom.ID+"\" x=\""+myBlock.x+"\" y=\""+myBlock.y+"\" flags=\""+myBlock.Flags+"\" doortime=\""+myBlock.DoorTime+"\" doorflags=\""+myBlock.DoorFlag+"\" jumpto_room=\""+myBlock.RoomLink+"\" jumpto_object=\""+nObjJump+"\" />");
					}

					//objects that are static and defined with link objects
					foreach(Block myBlock in lRoom.StaticItems)
					{
						sw.WriteLine("\t\t<roomobject id=\""+myBlock.SequenceCount.ToString()+"\" item=\""+myBlock.Number+"\" room=\""+lRoom.ID+"\" x=\""+myBlock.x+"\" y=\""+myBlock.y+"\" flags=\""+myBlock.Flags+"\" />");
					}
					//objects that are placed, e.g. food, etc.
					foreach(PlacedObject po in lRoom.PlacedItems)
					{						
						sw.WriteLine("\t\t<roomobject id=\""+po.SequenceCount.ToString()+"\" item=\""+po.Number+"\" room=\""+lRoom.ID+"\" x=\""+po.x+"\" y=\""+po.y+"\" />");
					}

				}
				sw.WriteLine("\t</roomobjects>");
				sw.WriteLine("");
				sw.WriteLine("\t<!-- DESCRIPTIONS FOR THE ITEMS REFERENCED IN THE MAP -->");
				sw.WriteLine("\t<!-- you may wish to extend them to make them useful for flags, etc -->");
				sw.WriteLine("\t<!-- ROOMTYPES: the different rooms you can have - id matches room id -->");
				sw.WriteLine("\t<!-- ROOMOBJECTS: the items placed in a room, e.g. doors, food, baddie, etc -->");
				sw.WriteLine("\t<!-- ROOMOBJECTTYPES: description of the types available -->");
				sw.WriteLine("\t<lookup_roomtypes>");
				foreach(BlockDescriptor rt in this.DefinedRooms)
				{
					if(rt.Number!=-1) sw.WriteLine("\t\t<roomtype id=\""+rt.Number+"\" bmp=\""+rt.bmp+"\" desc=\""+rt.Name+"\" />");
				}
				sw.WriteLine("\t</lookup_roomtypes>");
				sw.WriteLine("\t<lookup_roomobjects>");
				foreach(BlockDescriptor bd in this.DefinedBlocks)
					if(bd.Number!=-1) sw.WriteLine("\t\t<lookup_roomobject id=\""+bd.Number+"\" type=\""+bd.Type+"\" bmp=\""+bd.bmp+"\" desc=\""+bd.Name.Trim()+"\" ignoreifdoor=\""+(bd.IgnoreDoor?1:0)+"\" />");

				//placed objects
				int iLast=-1;
				foreach(PlacedObject po in this.PlacedObjects)
				{
					if(po.Number!=iLast)
					{
						sw.WriteLine("\t\t<lookup_roomobject id=\""+(po.Number)+"\" type=\""+po.Type+"\" bmp=\""+po.bmp+"\" desc=\""+po.desc.Trim()+"\" />");
						iLast=po.Number;
					}
				}


				sw.WriteLine("\t</lookup_roomobjects>");
				sw.WriteLine("");
				sw.WriteLine("\t<!--");
				sw.WriteLine("\t\tid is the main key used by 'item' above in the main body");
				sw.WriteLine("\t\ttype is the type of the object - see lookup_placeditemtypes");
				sw.WriteLine("\t\tkillsobject is if the item is used to kill another, e.g. cross kills dracula");
				sw.WriteLine("\t\tbmp and desc is there for extra detail");
				sw.WriteLine("\t-->");
				sw.WriteLine("\t<lookup_roomobjecttypes>");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"1\" desc=\"food\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"2\" desc=\"saps players energy\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"3\" desc=\"weapon to kill a baddie\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"4\" desc=\"baddie (devil) but free roaming cannot be killed\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"5\" desc=\"mummy - killed by leaf and placed with red key\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"6\" desc=\"frankenstein - killed by wrench\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"7\" desc=\"dracula - killed by cross\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"8\" desc=\"hunchback - killed by frog leg\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"9\" desc=\"special item?\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"10\" desc=\"door\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"11\" desc=\"door - clock\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"12\" desc=\"door - bookcase\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"13\" desc=\"door - barrel\" />");
				sw.WriteLine("\t\t<lookup_roomobjecttype id=\"14\" desc=\"static room object\" />");
				sw.WriteLine("\t</lookup_roomobjecttypes>");

				//finish
				sw.WriteLine("</map>");
				sw.Flush();
				sw.Close();
			}
			catch
			{
				return "Failed writing the main XML data or closing the file";
			}
			return "";	//success
		}
	}
	public class Room
	{
		public int Type;
		public int ID;						//room number - corresponds to grid
		public int Attribute;				//spectrum ink/background colour
		public ArrayList StaticItems;		//room objects
		public ArrayList RoomLinkItems;		//room objects
		public ArrayList PlacedItems;
		public Room()
		{
			StaticItems=new ArrayList();
			RoomLinkItems=new ArrayList();
			PlacedItems=new ArrayList();
		}
	};

	public struct PlacedObject
	{
		public int Number;
		public int x,y;
		public int Room;
		public int Type;
		public string Name;
		public string bmp;
		public string desc;
		public int SequenceCount;
	}

	public class Block
	{
		public int SequenceCount; //used to find the next/previous object if it is a room link item
		public int Number;	//type
		public int x,y;
		public int DoorTime;
		public int DoorFlag;
		public string Flags;
		public int iFlags;
		public int RoomLink;
		public Block() {RoomLink=-1;}
	};

	public struct BlockDescriptor
	{
		public int Number;
		public string Name;
		public string bmp;
		public int Type;
		public bool IgnoreDoor;	//hack really for tables that aren't really door links but described in data as doors
		public BlockDescriptor(int n, string na, int pm)
		{
			Number=n;
			Name=na;
			bmp="";
			Type=-1;
			IgnoreDoor=false;
		}
	};

}

