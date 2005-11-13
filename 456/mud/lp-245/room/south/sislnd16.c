reset(started)
{
    if (!started)
	set_light(1);
}

init()
{
    add_action("north", "north");
    add_action("south", "south");
    add_action("east", "east");
    add_action("west", "west");
}

short()
{
    return "Halfway up the hill on the Isle of the Magi";
}

long()
{
    write("You are halfway up the hill.\n" +
	  "On top of the hill, to the north, stands the ruins of the tower of\n" +
	  "Arcanarton.\n" +
	  "Paths wind down to the shore of the island to the south, east and west\n");
}

north()
{
    this_player()->move_player("north#room/south/sislnd18");
    return 1;
}

south()
{
    this_player()->move_player("south#room/south/sislnd7");
    return 1;
}

east()
{
    this_player()->move_player("east#room/south/sislnd6");
    return 1;
}

west()
{
    this_player()->move_player("west#room/south/sislnd8");
    return 1;
}
