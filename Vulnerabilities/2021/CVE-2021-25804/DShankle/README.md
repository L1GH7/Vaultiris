An AVI file can have multiple tracks. For example an audio track and a video track. 

In order to parse an arbitrary number of tracks VLC first must determine if a given track is finished and playable. Two variables are set up with this purpose in mind. Both named i_track.

The variable p_sys respresents the avi file for our purposes. Thus the "official" i_track variable which will be used to reference the tracks which are determined to be finished and playable is p_sys->i_track. It is initilized with a macro here:

    ```
    TAB_INIT(p_sys->i_track, p_sys->track);
    //#define TAB_INIT( count, tab ) do {(count) = 0; (tab) = NULL; } while(0)
    ```

Meanwhile a second i_track variable is set up to count ALL tracks regardless of whther or not they are finished/playable. 

    ```
    i_track = AVI_ChunkCount( p_hdrl, AVIFOURCC_strl, true );
    ```

The second i_track representing all chunks starting with 'strl'. It controls a 'for' loop which sets the p_sys->i_track variable only if it passes a series of checks:

    ```
    for( unsigned i = 0 ; i < i_track; i++ )
    {
        ...

    TAB_APPEND( p_sys->i_track, p_sys->track, tk );
    //#define TAB_APPEND( count, tab, p ) {TAB_APPEND_CAST( , count, tab, p )}
    //TAB_APPEND_CAST( ,p_sys->i_track, p_sys->track, tk)
            //if( (count) > 0 ) (tab) = cast realloc( tab, sizeof( *(tab) ) * ( (count) + 1 ) ); 
            //else  (tab) = cast malloc( sizeof( *(tab) ) );    
            //if( !(tab) ) abort();                       
            //(tab)[count] = (p);                         
            //(count)++; //p_sys->i_track incremented here
    }
    ```

During a subsequent 'for' loop the p_sys->i_track variable is utilized as the chunk # in a call to AVI_ChunkFind, which returns the nth chunk matching a given FOURCC chunk value, in our case 'strl':

    ```
    for( unsigned i = 0 ; i < p_sys->i_track; i++ )
    {
        avi_track_t         *tk = p_sys->track[i];

        if( tk->fmt.i_cat != AUDIO_ES ||
            tk->idx.i_size < 1 ||
            tk->i_scale != 1 ||
            tk->i_samplesize != 0 )
            continue;
            
        //CVE-2021-25804
        avi_chunk_list_t *p_strl = AVI_ChunkFind( p_hdrl, AVIFOURCC_strl, i, true );
        //subsequently changed to: avi_chunk_list_t *p_strl = AVI_ChunkFind( p_hdrl, AVIFOURCC_strl, tk->fmt.i_id, true );

        avi_chunk_strf_t *p_strf = AVI_ChunkFind( p_strl, AVIFOURCC_strf, 0, false );
        if( !p_strf || p_strf->i_cat != AUDIO_ES )
            continue;

        const WAVEFORMATEX *p_wf = p_strf->u.p_wf;
    ```

While p_sys->i_track does count the correct number of completed tracks it doesn't keep track of which chunk number it was set to. Meaning if there is an invalid strl chunk before the valid strl chunk the first chunk will correctly be ignored when setting up the track object but when ChunkFind is called it will still select the first, invalid chunk.

After CVE-2021-25804 was created the issue was patched by changing the ChunkFind chunk # variable from i to tk->fmt.i_id. tk->fmt.i_id is first set right above the TAB_APPEND() call we looked at earlier:

    ```
    tk->fmt.i_id = i;
    
    if( p_strn && p_strn->p_str )
        tk->fmt.psz_description = FromACP( p_strn->p_str );
    //see es_format_truncated.c 
    tk->p_es = es_out_Add( p_demux->out, &tk->fmt );
    

    TAB_APPEND( p_sys->i_track, p_sys->track, tk );
    ```

This seems to fix the issue as tk->fmt.i_id is set to i and thus the completed chunk should be the one selected by ChunkFind(). However testing reveals that in fact the value of tk->fmt.i_id is actually set to -1 at the time of the offending call to ChunkFind(). This is the intilization value set during a call to es_format_Init().

    ```
    void es_format_Init( es_format_t *fmt,
                     int i_cat, vlc_fourcc_t i_codec )
    {
        memset(fmt, 0, sizeof (*fmt));
        fmt->i_cat                  = i_cat;
        fmt->i_codec                = i_codec;
        fmt->i_profile              = -1;
        fmt->i_level                = -1;
        fmt->i_id                   = -1;
        fmt->i_priority             = ES_PRIORITY_SELECTABLE_MIN;
        fmt->psz_language           = NULL;
        fmt->psz_description        = NULL;
        fmt->p_extra_languages      = NULL;

        if (fmt->i_cat == VIDEO_ES)
            video_format_Init(&fmt->video, 0);

        fmt->b_packetized           = true;
        fmt->p_extra                = NULL;
    }

    ```

This initilization occurs in the same 'for' loop as fmt->i_id is set to i.

    ```
        for( unsigned i = 0 ; i < i_track; i++ )
    {
        ...
         switch( p_strh->i_type )
        {
            case( AVIFOURCC_auds ):
            {
                es_format_Init( &tk->fmt, AUDIO_ES, 0 );
                ...
            }
                ...
        }
        tk->fmt.i_id = i;

        if( p_strn && p_strn->p_str )
            tk->fmt.psz_description = FromACP( p_strn->p_str );

        tk->p_es = es_out_Add( p_demux->out, &tk->fmt );

        TAB_APPEND( p_sys->i_track, p_sys->track, tk );
    }

    ```



