#include "../gorillagame.h"

void IntroState::Init()
{
    if( !font )
    {
        font = TTF_OpenFont( BasePath "asset/font/RobotoSlab-Bold.ttf", 24 );
        TTF_SetFontHinting( font, TTF_HINTING_LIGHT );
        if( !font )
            cerr << "TTF_OpenFont failed: " << TTF_GetError() << endl;
    }

    if( !image )
    {
        image = IMG_LoadTexture( render, BasePath "asset/graphic/background.png" );
        if( !image )
            cerr << "IMG_LoadTexture failed: " << IMG_GetError() << endl;
    }

    if( !music )
    {
        music = Mix_LoadMUS( BasePath "asset/music/severance.ogg" );
        if( !music )
            cerr << "Mix_LoadMUS failed: " << Mix_GetError() << endl;
        else
            Mix_PlayMusic( music, -1 );
    }
    else if( Mix_PausedMusic() )
    {
        Mix_ResumeMusic();
    }
}

void IntroState::UnInit()
{
    if( !Mix_PausedMusic() )
        Mix_PauseMusic();

    // Keep everything loaded/allocated is also an option
    /*
    TTF_CloseFont( font );
    SDL_DestroyTexture( image );
    SDL_DestroyTexture( blendedText );
    font = nullptr;
    image = nullptr;
    blendedText = nullptr;
    */
}

void IntroState::Events( const u32 frame, const u32 totalMSec, const float deltaT )
{
    SDL_PumpEvents();

    Event event;
    while( SDL_PollEvent( &event ) )
    {
        if( game.HandleEvent( event ) )
            continue;

        switch( event.type )
        {
            case SDL_KEYDOWN:
            {
                const Keysym & what_key = event.key.keysym;

                if( what_key.scancode == SDL_SCANCODE_F1 && event.key.repeat == 0 )
                {
                    if( Mix_PausedMusic() )
                        Mix_ResumeMusic();
                    else
                        Mix_PauseMusic();

                }
                else if( what_key.scancode == SDL_SCANCODE_F2 && event.key.repeat == 0 )
                {
                    if( Mix_VolumeMusic( -1 ) == MIX_MAX_VOLUME )
                        Mix_VolumeMusic( 0 );
                    else
                        Mix_VolumeMusic( MIX_MAX_VOLUME );

                }
                else if( what_key.scancode == SDL_SCANCODE_F9 )
                {
                    // crash/shutdown, since State #6 does not exist
                    game.SetNextState( 99 );
                }
                else if( what_key.scancode == SDL_SCANCODE_ESCAPE )
                {
                    game.SetNextState( 0 );
                }

                break;
            }

            case SDL_MOUSEBUTTONDOWN:
                game.SetNextState( 1 );
                break;

            default:
                break;
        }
    }
}

void IntroState::Update( const u32 frame, const u32 totalMSec, const float deltaT )
{
}

void IntroState::Render( const u32 frame, const u32 totalMSec, const float deltaT )
{
    const Point & winSize = game.GetWindowSize();

    {
        const Rect dst_rect { 0, 0, winSize.x, winSize.y };
        SDL_RenderCopy( render, image, EntireRect, &dst_rect /* same result as EntireRect */ );
    }

    // Poor persons benchmark
    //for (uint x = 0; x < 100; ++x)
    {
        // uncomment to use the cache, comment out to disable
        if( blendedText == nullptr )
        {
            constexpr const char * text =
                    "                                          --== Introscreen of my Super Mega Gamey Game 3000 ==--\n\n"
                    "Dies ist ein Typoblindtext. An ihm kann man sehen, ob alle Buchstaben da sind und wie sie aussehen. Manchmal benutzt man Worte wie Hamburgefonts, Rafgenduks oder Handgloves, um Schriften zu testen. Manchmal Sätze, die alle Buchstaben des Alphabets enthalten - man nennt diese Sätze »Pangrams«. Sehr bekannt ist dieser: The quick brown fox jumps over the lazy old dog. Oft werden in Typoblindtexte auch fremdsprachige Satzteile eingebaut (AVAIL® and Wefox™ are testing aussi la Kerning), um die Wirkung in anderen Sprachen zu testen. In Lateinisch sieht zum Beispiel fast jede Schrift gut aus. Quod erat demonstrandum. Seit 1975 fehlen in den meisten Testtexten die Zahlen, weswegen nach TypoGb. 204 § ab dem Jahr 2034 Zahlen in 86 der Texte zur Pflicht werden. Nichteinhaltung wird mit bis zu 245 € oder 368 $ bestraft. Genauso wichtig in sind mittlerweile auch Âçcèñtë, die in neueren Schriften aber fast immer enthalten sind. Ein wichtiges aber schwierig zu integrierendes Feld sind OpenType-Funktionalitäten. Je nach Software und Voreinstellungen können eingebaute Kapitälchen, Kerning oder Ligaturen (sehr pfiffig) nicht richtig dargestellt werden."
                    "\n\nRoyality free music by Karl Casey @ White Bat Audio\n  - Press [F1] to (un)pause music.\n  - Press [F2] to (un)mute music.\nSource: https://www.youtube.com/watch?v=aFITtvK64B4"
                    "\n\nPress any key to continue!";

            if( blendedText != nullptr )
                SDL_DestroyTexture( blendedText );

            Surface * surf = TTF_RenderUTF8_Blended_Wrapped( font, text, white, winSize.x - 30 );
            blendedText = SDL_CreateTextureFromSurface( render, surf );
            SDL_FreeSurface( surf );

            u32 fmt;
            int access;
            SDL_QueryTexture( blendedText, &fmt, &access, &blendedTextSize.x, &blendedTextSize.y );
        }

        // Draw the text on top
        {
            constexpr const Point p { 32, 50 };
            SDL_SetTextureColorMod( blendedText, 0, 0, 0 );
            for( const Point & pd : shadowOffsets )
            {
                const Rect dst_rect = Rect{ p.x + pd.x, p.y + pd.y, blendedTextSize.x, blendedTextSize.y };
                SDL_RenderCopy( render, blendedText, EntireRect, &dst_rect );
            }

            SDL_SetTextureColorMod( blendedText, 255, 255, 255 );
            const Rect dst_rect = { p.x, p.y, blendedTextSize.x, blendedTextSize.y };
            SDL_RenderCopy( render, blendedText, EntireRect, &dst_rect );
        }
    }
}