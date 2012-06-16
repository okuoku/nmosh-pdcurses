/*
 *  newtest.c   -   Tests certain PDCurses functions,
 *    mostly those in Win32a,  including some of the
 *    new attributes for 64-bit chtype.  I wanted to be
 *    sure the PDC_set_blink and PDC_set_line_color
 *    functions worked,  and that A_OVERLINE and A_STRIKEOUT
 *    displayed properly.  Also tests "extended" SLK functions.
 *
 */

#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>

static const char *labels[] = {
               "Quit", "Blink", "431", "2134", "55",
               "62", "83", "7", "b", "25",
               "Able", "Baker", "Charlie", "Dog",
               "Easy", "Fox", "Golf", "How", "Item",
               "Jig", "King", "Love", "Mike", "Nan",
               "Oboe", "Peter", "Queen", "Roger", "Sugar",
               "Tear", "Uncle", "Victor", "Whiskey",
               "X-Ray", "Yoke", "Zebra", NULL };


static void slk_setup( const int slk_format)
{
    int i;
    static int old_format = 0xa;

    if( slk_format != old_format)
       slk_init( slk_format);
    old_format = slk_format;
    for( i = 0; labels[i]; i++)
       slk_set( i + 1, labels[i], 1);
    slk_refresh( );
}

   /* Uses the left/right/under/overline capabilities of Win32a */
   /* to ensure the text is "boxed".  */

void text_in_a_box( const char *istr)
{
   const int len = strlen( istr);

   attron( A_OVERLINE | A_UNDERLINE | A_LEFTLINE);
   if( len == 1)
      attron( A_RIGHTLINE);
   addnstr( istr, 1);
   if( len > 1)
      {
      attroff( A_LEFTLINE);
      if( len > 2)
         addnstr( istr + 1, len - 2);
      attron( A_RIGHTLINE);
      addnstr( istr + len - 1, 1);
      }
   attroff( A_OVERLINE | A_UNDERLINE | A_LEFTLINE | A_RIGHTLINE);
}

#define COL1 2
#define COL2 (COL1 + 30)
#define COL3 72

#define N_CURSORS 9
      /* There are nine different cursor types;  see below for the list.  */
      /* You specify two types,  and the cursor blinks between them.      */
      /* Default is between "underline" and "invisible".  Set both states */
      /* to the same value to get an unblinking cursor.                   */

#define PURE_WINDOWS_VERSION  1

/* Among other things,  'newtest' demonstrates how to make a Win32a
PDCurses app that is a for-real,  "pure Windows" version (instead of
a console application).  Doing this is quite easy,  and has certain
advantages.  If the app is invoked from a command prompt,  the only
difference you'll see is that the app runs separately (that is,  you
can continue to use the command prompt,  switching between it,  your
PDCurses/Win32a app,  and other processes).  Which is the main reason
I did it;  it meant that I could invoke a PDCurses-based text editor,
for example,  and still have use of the command line.

   (NOTE that,  for reasons I don't actually understand,  this happens
when the Visual C++ compiler is used.  With MinGW or OpenWatcom,  it's
still "really" a console app.)

   To do it,  we ensure that the usual main() function has an alternative
dummy_main() form,  taking the same arguments as main().  We add a
WinMain() function,  whose sole purpose is to reformulate lpszCmdLine
into argc/argv form,  and pass it on to dummy_main().  And,  of course,
we can switch back to a "normal" console app by removing the above
#define PURE_WINDOWS_VERSION line.             */

#ifdef PURE_WINDOWS_VERSION
#include <windows.h>

int dummy_main( int argc, char **argv);

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszCmdLine, int nCmdShow)
{
   char *argv[30];
   int i, argc = 1;

   argv[0] = "newtest";
   for( i = 0; lpszCmdLine[i]; i++)
       if( lpszCmdLine[i] != ' ' && (!i || lpszCmdLine[i - 1] == ' '))
          argv[argc++] = lpszCmdLine + i;

   for( i = 0; lpszCmdLine[i]; i++)
       if( lpszCmdLine[i] == ' ')
          lpszCmdLine[i] = '\0';

   return dummy_main( argc, (char **)argv);
}

int dummy_main( int argc, char **argv)
#else       /* "usual",  console-app version: */
int main( int argc, char **argv)
#endif
{
    int quit = 0, i, fmt = 0xa, use_slk = 1;
    bool blink_state = FALSE;
    int cursor_state_1 = 1, cursor_state_2 = 0;
    int show_slk_index_line = 0;

    setlocale(LC_ALL, ".utf8");
    for( i = 1; i < argc; i++)
        if( argv[i][0] == '-')
            switch( argv[i][1])
            {
                case 's':
                    use_slk = 0;
                    break;
                case 'l':
                    setlocale( LC_ALL, argv[i] + 2);
                    break;
                case 'f':
                    sscanf( argv[i] + 2, "%x", &fmt);
                    break;
                case 'i':
                    show_slk_index_line = 1;
                    break;
                default:
                    printf( "Option '%s' unrecognized\n", argv[i]);
                    break;
            }
    if( use_slk)
       slk_init( show_slk_index_line ? 3 : 0);
    Xinitscr(argc, argv);
    if( use_slk)
       slk_setup( show_slk_index_line ? -fmt : fmt);

    start_color();

# if defined(NCURSES_VERSION) || (defined(PDC_BUILD) && PDC_BUILD > 3000)
    use_default_colors();
# endif
    cbreak();
    noecho();
    clear();
    refresh();
#ifdef __PDCURSES__
    PDC_set_title( "NewTest: tests various PDCurses features");
#endif
    keypad( stdscr, TRUE);
    init_pair( 1, COLOR_WHITE, COLOR_BLACK);
    init_pair( 2, COLOR_BLACK, COLOR_YELLOW);
    for( i = 3; i < 256; i++)
        init_pair( (short)i, (short)(i - 3), COLOR_BLACK);

    attrset( COLOR_PAIR( 1));
    mvaddstr( 2, COL1, "'Normal' white-on-black");
    attron( A_BLINK);
    mvaddstr( 2, COL2, "Blinking");
    attron( A_BOLD);
    mvaddstr( 3, COL1, "BlinkBold");
    attron( A_ITALIC);
    mvaddstr( 3, COL2, "BlinkBoldItalic");
    attrset( COLOR_PAIR( 5));
    attron( A_UNDERLINE);
    mvaddstr( 4, COL1, "Underlined");
    attrset( COLOR_PAIR( 1));
    attron( A_UNDERLINE | A_ITALIC);
    mvaddstr( 4, COL2, "UnderlinedItalic");

    attrset( COLOR_PAIR( 2));
    attron( A_BLINK);
    mvaddstr( 6, COL1, "Black-on-yellow blinking");

    attrset( COLOR_PAIR( 1));
    move( 6, COL2);
    text_in_a_box( "Text in a box");


    attrset( COLOR_PAIR( 6));
    attron( A_STRIKEOUT);
    mvaddstr( 0, COL2, "Strikeout");

    attrset( COLOR_PAIR( 1));
    for( i = 0; i < 128; i++)
    {                 /* Show extended characters: */
        char buff[5];

        sprintf( buff, "%2x %c", i + 128, (char)(i + 128));
        mvaddstr( 8 + i % 16, (i / 16) * 5 + 1, buff);
    }

    for( i = 0; i < 3; i++)
    {                 /* Demonstrate full RGB color control: */
        int j;
        const char *output_text[3] = {
            "Red on green to white on black   | (you can get any colors you",
            "Blue on yellow to black on green | want without palette issues)",
            "White on red to green on gray,  underlined and italic" };
        const int len = strlen( output_text[i]);

        move( 24 + i, 1);
        for( j = 0; j < len; j++)
        {
            attr_t output_color;
            int oval = j * 31 / len;

            if( !i)
               output_color = A_RGB( 31, oval, oval, 0, 31 - oval, 0);
            else if( i == 1)
               output_color = A_RGB( 0, 0, 31 - oval, 31 - oval, 31, 0);
            else
               output_color = A_RGB( 31 - oval, 31, 31 - oval,
                       31 - oval * 2 / 3, oval / 3, oval / 3)
                      | A_UNDERLINE | A_ITALIC;
            attrset( output_color);
            addch( output_text[i][j]);
        }
    }


    mousemask( ALL_MOUSE_EVENTS, NULL);

    while( !quit)
    {
        char buff[40];
        const int color_block_start = 49;
        int c;
        const char *cursor_state_text[N_CURSORS] = {
                  "Invisible cursor  ",
                  "Underscore cursor ",
                  "Block cursor      ",
                  "Outline box cursor",
                  "Caret cursor      ",
                  "Half-block cursor ",
                  "Central block curs",
                  "Cross cursor      ",
                  "Heavy box cursor  " };

        attrset( COLOR_PAIR( 1));

#ifdef MAYBE_TRY_THIS_SOMEWHERE_ELSE
        mvaddstr(  1, COL3, "Click on cursor descriptions to");
        mvaddstr(  2, COL3, "cycle through possible cursors");
        mvaddstr(  3, COL3, "Click on colors at left to change");
        mvaddstr(  4, COL3, "colors used for under/over/outlining");
        mvaddstr(  5, COL3, "Click 'Blink' at bottom to toggle");
        mvaddstr(  6, COL3, "'real' blinking vs. 'highlit' blink");
#endif

        mvaddstr( 22, COL3, cursor_state_text[cursor_state_1]);
        mvaddstr( 23, COL3, cursor_state_text[cursor_state_2]);

        for( i = 11; i < 256; i++)
        {
            attrset( COLOR_PAIR( i));
            init_pair((short)i, (short)i, COLOR_BLACK);
            if( !(i % 11))
               move( i / 11, color_block_start);
            attron( A_REVERSE);
            addstr( "  ");
        }
        refresh();
        c = getch( );
        attrset( COLOR_PAIR( 1));
        if( c == KEY_RESIZE)
            resize_term( 0, 0);
        else if( c == KEY_F(1) || c == 27)
            quit = 1;
        else if( c == KEY_F(2))
        {
            blink_state ^= 1;
            PDC_set_blink( blink_state);
        }
        else if( c >= KEY_F(3) && c < KEY_F(12))
        {
            sscanf( labels[c - KEY_F(1)], "%x", &fmt);
            if( use_slk)
                slk_setup( show_slk_index_line ? -fmt : fmt);
        }
        if( c != KEY_MOUSE)
        {
            sprintf( buff, "Key %s hit      ", keyname( c));
            mvaddstr( 0, COL1, buff);
        }
        else
        {
            MEVENT mouse_event;
#ifdef __PDCURSES__
            nc_getmouse( &mouse_event);
#else
            getmouse( &mouse_event);
#endif
            sprintf( buff, "Mouse at %d x %d    ", mouse_event.x, mouse_event.y);
            mvaddstr( 0, COL1, buff);
            if( mouse_event.x >= color_block_start && mouse_event.y < 24
                           && mouse_event.y > 0)
               {
               int new_color = (mouse_event.x - color_block_start) / 2;

               if( new_color > 10)
                  {
                  if( mouse_event.y == 22)  /* blink/non-blink toggle */
                     cursor_state_1 = (cursor_state_1 + 1) % N_CURSORS;
                  else if( mouse_event.y == 23)  /* cycle cursor state */
                     cursor_state_2 = (cursor_state_2 + 1) % N_CURSORS;
                  else
                     new_color = -1;
                  curs_set( (cursor_state_1 << 8) | cursor_state_2);
                  }
               else
                  {
                  new_color += mouse_event.y * 11;
                  if( new_color >= 256)
                     new_color = -1;
                  }
               PDC_set_line_color( (short)new_color);
               }
        }
    }

    endwin();

    return 0;
}

