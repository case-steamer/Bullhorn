/*
Bullhorn version 0.0
Produced by James Haining

    Bullhorn is meant to be a simple software for small event organizers to be 
    able to automate loudspeaker announcements. Other software for similar uses 
    is available, but they are costly and/or overpowered for what the average, 
    non-technical organizer might need. Bullhorn aims to fill this gap.

    Programs for Bullhorn are organized as a "Queue", which is made up of "Blocks". 
    A block is meant to be made up of many audio files which can be scheduled to 
    run at a specific time in the queue. Thus, the user could have two similar 
    blocks air at different times of day, with audio files added or removed appropriate 
    to the time of day.

    EXAMPLE:
    If your event is going to have specific timed activities, you can insert an 
    announcement for it at the appropriately timed block. However, you might also
    have sponsors or food vendors that you want to promote repeatedly; in this case,
    you can record one advertisement for your vendor, and then insert that audio
    file into multiple blocks to be scheduled through the day.

    An example of a block would look like this:

-block scheduled for 11:00 am
    -face_painting.mp3
    -band_performance.mp3
    -corporate_sponsor.mp3

    Blocks are scheduled on 24 hour time. Inside the queue list, you will see a
    small checkbox that says "override". This is where you determine if block B
    should interrupt block A if they should happen to overlap. If this box is not
    checked, block B will not perform until block A has run fully.

    At this time, Bullhorn does not have support for m4a files. A future version of
    this program will correct that, but for now m4a files will have to be converted
    to a supported file type. Currently supported filetypes are .mp3, .wav, & .flac.
    Why this matters to the end user is that while it would be very convenient to
    be able to simply import voice memos from a users smart phone, at this time that
    functionality is not there. A future version will fix this.
*/

#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "XMLIO.h"
#include "Driver.h"
#include "MainWindow.h"
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        XMLIO       xmlio;
        Driver      driver(xmlio);
        MainWindow  window(driver);
        window.run();
    }

    catch (const std::runtime_error& e)
    {
        std::cout << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
        return 0;
}

