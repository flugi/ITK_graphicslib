//
//  SDLApplication.m
//  ITK_graphicslib
//
//  Created by Olivér Kocsis on 2012.06.08..
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "SDLApplication.h"
#include <SDL/SDL.h>

@implementation SDLApplication
/* Invoked from the Quit menu item */
- (void)terminate:(id)sender
{
    /* Post a SDL_QUIT event */
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}
@end
