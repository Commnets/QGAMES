// console.cpp : Defines the entry point for the console application.
//

#include "tinyxml\tinyxml.h"

int doerror(char* str)
{
	printf("\n%s",str);
	return 1;
}


void processdata(TiXmlDocument*);

int main(int argc, char* argv[])
{
	//open and parse document
	TiXmlDocument* doc=new TiXmlDocument( "outputs/aa_xml.xml" );
	if(!doc->LoadFile())
	{
		printf("cannot open file or it fails parsing (invalid XML)\n");
		return 1;
	}

	processdata(doc);
	delete doc;

	return 0;
}

void processdata(TiXmlDocument* doc)
{
	//top level elements
	TiXmlElement* map;
	TiXmlElement* acgkey;
	TiXmlElement* yellowkey;
	TiXmlElement* greenkey;
	TiXmlElement* redkey;
	TiXmlElement* cyankey;
	TiXmlElement* rooms;
	TiXmlElement* roomobjects;
	//top level lookup tables
	TiXmlElement* lookuproomtypes;
	TiXmlElement* lookuproomobjects;
	TiXmlElement* lookuproomobjecttypes;

	//resusable elements for any given node
	TiXmlElement* node;	//the actual element
	TiXmlNode* nodule;	//the item within
	const char* returned;		//value returned - NULL if not
	char c;

	printf("\nTo redirect to a file just run console.exe > file.txt");
	printf("\nBut remember to press ENTER lots of times until it is finished");
	printf("\nPress ENTER to continue\n");
	scanf("%c",&c);
	//get pointer to the map - the whole xml file
	map=doc->RootElement();

	//get the top level elements
	acgkey=map->FirstChildElement("acg_keylocations")->ToElement();
		if(acgkey==NULL) doerror("acgkeys do not exist");

	//no more error checking from now on - the above is how it is done
	yellowkey=map->FirstChildElement("yellowkey")->ToElement();
	greenkey=map->FirstChildElement("greenkey")->ToElement();
	redkey=map->FirstChildElement("redkey")->ToElement();
	cyankey=map->FirstChildElement("cyankey")->ToElement();
	rooms=map->FirstChildElement("rooms")->ToElement();
	roomobjects=map->FirstChildElement("roomobjects")->ToElement();
	lookuproomtypes=map->FirstChildElement("lookup_roomtypes")->ToElement();
	lookuproomobjects=map->FirstChildElement("lookup_roomobjects")->ToElement();
	lookuproomobjecttypes=map->FirstChildElement("lookup_roomobjecttypes")->ToElement();

	//get the acg keys
	int left,middle,right;
	for( nodule= acgkey->FirstChild( "acg_keylocation" );
		 nodule;
		 nodule = nodule->NextSibling( "acg_keylocation" ) )
	{
		node=nodule->ToElement();
		returned=node->Attribute("left",&left);
		returned=node->Attribute("middle",&middle);
		returned=node->Attribute("right",&right);

		printf("\nacg key posible locations: left %d, middle %d, right %d",left,middle,right);
	}

	//get the yellow key
	int key1,key2,key3,key4,key5,key6,key7,key8;
	returned=yellowkey->Attribute("room1",&key1);
		printf("\n\nyellow key is in room %d",key1);

	//ignoring nulls as you get the picture :)
	//red key 
	returned=redkey->Attribute("room1",&key1);
	returned=redkey->Attribute("room2",&key2);
	returned=redkey->Attribute("room3",&key3);
	returned=redkey->Attribute("room4",&key4);
	returned=redkey->Attribute("room5",&key5);
	returned=redkey->Attribute("room6",&key6);
	returned=redkey->Attribute("room7",&key7);
	returned=redkey->Attribute("room8",&key8);
	printf("\nred key is in one of these: %d %d %d %d %d %d %d %d",key1,key2,key3,key4,key5,key6,key7,key8);
	//green key 
	returned=greenkey->Attribute("room1",&key1);
	returned=greenkey->Attribute("room2",&key2);
	returned=greenkey->Attribute("room3",&key3);
	returned=greenkey->Attribute("room4",&key4);
	returned=greenkey->Attribute("room5",&key5);
	returned=greenkey->Attribute("room6",&key6);
	returned=greenkey->Attribute("room7",&key7);
	returned=greenkey->Attribute("room8",&key8);
	printf("\ngreen key is in one of these: %d %d %d %d %d %d %d %d",key1,key2,key3,key4,key5,key6,key7,key8);
	//cyan key 
	returned=cyankey->Attribute("room1",&key1);
	returned=cyankey->Attribute("room2",&key2);
	returned=cyankey->Attribute("room3",&key3);
	returned=cyankey->Attribute("room4",&key4);
	returned=cyankey->Attribute("room5",&key5);
	returned=cyankey->Attribute("room6",&key6);
	returned=cyankey->Attribute("room7",&key7);
	returned=cyankey->Attribute("room8",&key8);
	printf("\ncyan key is in one of these: %d %d %d %d %d %d %d %d",key1,key2,key3,key4,key5,key6,key7,key8);
	printf("\n\nPress ENTER to continue to the rooms");
	scanf("%c",&c);

	//get the rooms
	int id, type;
	for( nodule= rooms->FirstChild( "room" );
		 nodule;
		 nodule = nodule->NextSibling( "room" ) )
	{
		node=nodule->ToElement();
		returned=node->Attribute("id",&id);
		returned=node->Attribute("type",&type);

		printf("\nroom number %d is of type %d",id,type);
	}
	printf("\n\nThe types can be seen in the lookups later. Press ENTER for the objects in rooms");
	scanf("%c",&c);

	//room objects
	int roomid, item,room,x,y,doortime,jumptoroom,jumptoobject;
	//int doorflags;
	const char* flags="";
	for( nodule= roomobjects->FirstChild( "roomobject" );
		 nodule;
		 nodule = nodule->NextSibling( "roomobject" ) )
	{
		node=nodule->ToElement();
		returned=node->Attribute("id",&roomid);
		returned=node->Attribute("item",&item);
		returned=node->Attribute("room",&room);
		returned=node->Attribute("x",&x);
		returned=node->Attribute("y",&y);
		//optional
		flags=node->Attribute("flags");
		returned=node->Attribute("doortime",&doortime);
			if(returned==NULL) doortime=-1;
		//returned=node->Attribute("doorflags",&doorflags);
		returned=node->Attribute("jumpto_room",&jumptoroom);
			if(returned==NULL) jumptoroom=-1;
		returned=node->Attribute("jumpto_object",&jumptoobject);
			if(returned==NULL) jumptoobject=-1;

		printf("\nobj %d is itemtype %d,x=%d,y=%d ",roomid,item,x,y);
		if(doortime!=-1) printf("doortime=%d,jumptoroom=%d,jumptoobject=%d",doortime,jumptoroom,jumptoobject);
	}
	printf("\n\nThe item types can be seen in the lookups. Press ENTER for the lookup values");
	scanf("%c",&c);

	//lookups
	//	roomtype
	const char* bmp;
	const char* desc;
	for( nodule= lookuproomtypes->FirstChild( "roomtype" );
		 nodule;
		 nodule = nodule->NextSibling( "roomtype" ) )
	{
		node=nodule->ToElement();
		returned=node->Attribute("id",&id);
		bmp=node->Attribute("bmp");
		desc=node->Attribute("desc");
		if(id!=-1) printf("\nlookup room type %d: bmp=%s, desc=%s",id,bmp,desc);
	}
	printf("\n\nThe room types are probably not necessary. just give an indication of the room type. Press ENTER for the objects in rooms");
	scanf("%c",&c);

	//	roomobjects
	for( nodule= lookuproomobjects->FirstChild( "lookup_roomobject" );
		 nodule;
		 nodule = nodule->NextSibling( "lookup_roomobject" ) )
	{
		node=nodule->ToElement();
		returned=node->Attribute("id",&id);
		returned=node->Attribute("type",&type);
		bmp=node->Attribute("bmp");
		desc=node->Attribute("desc");
		if(id!=-1) printf("\nlookup object id %d: type=%d, bmp=%s, desc=%s",id,type,bmp,desc);
	}
	printf("\n\nObject types shows what it is. described next. Press ENTER.");
	scanf("%c",&c);

	//	roomobjecttypes
	for( nodule= lookuproomobjecttypes->FirstChild( "lookup_roomobjecttype" );
		 nodule;
		 nodule = nodule->NextSibling( "lookup_roomobjecttype" ) )
	{
		node=nodule->ToElement();
		returned=node->Attribute("id",&id);
		desc=node->Attribute("desc");
		if(id!=-1) printf("\nlookup object type %d: desc=%s",id,desc);
	}
	printf("\n\nObject type probably not needed and just kept in code");
}

