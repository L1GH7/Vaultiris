//initial decleration
    i_track = AVI_ChunkCount( p_hdrl, AVIFOURCC_strl, true );
    if( p_avih->i_streams != i_track )
    {
        msg_Warn( p_demux,
                  "found %d stream but %d are declared",
                  i_track, p_avih->i_streams );
    }
    if( i_track == 0 )
    {
        msg_Err( p_demux, "no stream defined!" );
        goto error;
    }

    /* print information on streams */
    msg_Dbg( p_demux, "AVIH: %d stream, flags %s%s%s%s ",
             i_track,
             p_avih->i_flags&AVIF_HASINDEX?" HAS_INDEX":"",
             p_avih->i_flags&AVIF_MUSTUSEINDEX?" MUST_USE_INDEX":"",
             p_avih->i_flags&AVIF_ISINTERLEAVED?" IS_INTERLEAVED":"",
             p_avih->i_flags&AVIF_TRUSTCKTYPE?" TRUST_CKTYPE":"" );

    p_sys->b_interleaved |= (p_avih->i_flags & AVIF_ISINTERLEAVED);

    /* Set callbacks */
    p_demux->pf_control = Control;

    if( p_sys->b_fastseekable )
    {
        p_demux->pf_demux = Demux_Seekable;
        p_sys->i_read_increment = READ_LENGTH;
    }
    else if( p_sys->b_seekable )
    {
        p_demux->pf_demux = Demux_Seekable;
        p_sys->i_read_increment = READ_LENGTH_NONINTERLEAVED;
        if( !p_sys->b_interleaved )
            msg_Warn( p_demux, "Non interleaved content over slow seekable, "
                               "expect bad performance" );
    }
    else
    {
        msg_Warn( p_demux, "Non seekable content " );

        p_demux->pf_demux = Demux_UnSeekable;
        p_sys->i_read_increment = READ_LENGTH_NONINTERLEAVED;
         /* non seekable and non interleaved case ? well... */
        if( !p_sys->b_interleaved )
        {
            msg_Warn( p_demux, "Non seekable non interleaved content, "
                               "disabling other tracks" );
            i_track = __MIN(i_track, 1);
        }
    }

    AVI_MetaLoad( p_demux, p_riff, p_avih );
    p_sys->i_avih_flags = p_avih->i_flags;

    /* now read info on each stream and create ES */
    for( unsigned i = 0 ; i < i_track; i++ )
    {
        avi_track_t           *tk     = calloc( 1, sizeof( avi_track_t ) );
        if( unlikely( !tk ) )
            goto error;

        avi_chunk_list_t      *p_strl = AVI_ChunkFind( p_hdrl, AVIFOURCC_strl, i, true );  
        avi_chunk_strh_t      *p_strh = AVI_ChunkFind( p_strl, AVIFOURCC_strh, 0, false ); //stream header
        avi_chunk_STRING_t    *p_strn = AVI_ChunkFind( p_strl, AVIFOURCC_strn, 0, false ); //stream name (optional?)
        avi_chunk_strf_t      *p_strf = AVI_ChunkFind( p_strl, AVIFOURCC_strf, 0, false ); //stream format

        tk->b_eof = false;
        tk->b_activated = true;

        if( p_strl == NULL || p_strh == NULL || p_strf == NULL )
        {
            msg_Warn( p_demux, "stream[%d] incomplete", i );
            free( tk );  
            continue;
        }

        tk->i_rate  = p_strh->i_rate;
        tk->i_scale = p_strh->i_scale;
        tk->i_samplesize = p_strh->i_samplesize;
        msg_Dbg( p_demux, "stream[%u] rate:%u scale:%u samplesize:%u",
                i, tk->i_rate, tk->i_scale, tk->i_samplesize );
        if( tk->i_scale > tk->i_rate || !tk->i_scale || !tk->i_rate )
        {
            free( tk );
            continue;
        }

        switch( p_strh->i_type )
        {
            case( AVIFOURCC_auds ):
            {
                if(unlikely( p_strf->i_cat != AUDIO_ES ))
                {
                    free(tk);
                    continue;
                }
                es_format_Init( &tk->fmt, AUDIO_ES, 0 );

                WAVEFORMATEX *p_wf = p_strf->u.p_wf;

                if( p_wf->wFormatTag == WAVE_FORMAT_EXTENSIBLE &&
                    p_wf->cbSize >= sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX) )
                {
                    WAVEFORMATEXTENSIBLE *p_wfe = (WAVEFORMATEXTENSIBLE *)p_wf;
                    tk->fmt.i_codec = AVI_FourccGetCodec( AUDIO_ES, p_wfe->SubFormat.Data1 );
                }
                else
                    tk->fmt.i_codec = AVI_FourccGetCodec( AUDIO_ES, p_wf->wFormatTag );

                tk->i_blocksize = p_wf->nBlockAlign;
                if( tk->i_blocksize == 0 )
                {
                    if( p_wf->wFormatTag == 1 )
                        tk->i_blocksize = p_wf->nChannels * (p_wf->wBitsPerSample/8);
                    else
                        tk->i_blocksize = 1;
                }
                else if( tk->i_samplesize != 0 && tk->i_samplesize != tk->i_blocksize )
                {
                    msg_Warn( p_demux, "track[%u] samplesize=%u and blocksize=%u are not equal."
                                       "Using blocksize as a workaround.",
                                       i, tk->i_samplesize, tk->i_blocksize );
                    tk->i_samplesize = tk->i_blocksize;
                }

                if( tk->fmt.i_codec == VLC_CODEC_VORBIS )
                {
                    tk->i_blocksize = 0; /* fix vorbis VBR decoding */
                }

                if ( tk->fmt.i_codec == VLC_CODEC_MP4A )
                {
                    tk->i_samplesize = 0; /* ADTS/AAC VBR */
                }

                /* Fix broken scale/rate */
                if ( tk->fmt.i_codec == VLC_CODEC_ADPCM_IMA_WAV &&
                     tk->i_samplesize && tk->i_samplesize > tk->i_rate )
                {
                    tk->i_scale = 1017;
                    tk->i_rate = p_wf->nSamplesPerSec;
                }

                /* From libavformat */
                /* Fix broken sample size (which is mp2 num samples / frame) #12722 */
                if( tk->fmt.i_codec == VLC_CODEC_MPGA &&
                    tk->i_samplesize == 1152 && p_wf->nBlockAlign == 1152 )
                {
                    p_wf->nBlockAlign = tk->i_samplesize = 0;
                }

                tk->fmt.audio.i_channels        = p_wf->nChannels;
                tk->fmt.audio.i_rate            = p_wf->nSamplesPerSec;
                tk->fmt.i_bitrate               = p_wf->nAvgBytesPerSec*8;
                tk->fmt.audio.i_blockalign      = p_wf->nBlockAlign;
                tk->fmt.audio.i_bitspersample   = p_wf->wBitsPerSample;
                tk->fmt.b_packetized            = !tk->i_blocksize;

                avi_chunk_list_t *p_info = AVI_ChunkFind( p_riff, AVIFOURCC_INFO, 0, true );
                if( p_info )
                {
                    int i_chunk = AVIFOURCC_IAS1 + ((i - 1) << 24);
                    avi_chunk_STRING_t *p_lang = AVI_ChunkFind( p_info, i_chunk, 0, false );
                    if( p_lang != NULL && p_lang->p_str != NULL )
                        tk->fmt.psz_language = FromACP( p_lang->p_str );
                }

                msg_Dbg( p_demux,
                    "stream[%u] audio(0x%x - %s) %d channels %dHz %dbits",
                    i, p_wf->wFormatTag,
                    vlc_fourcc_GetDescription(AUDIO_ES, tk->fmt.i_codec),
                    p_wf->nChannels,
                    p_wf->nSamplesPerSec,
                    p_wf->wBitsPerSample );

                const size_t i_cboff = sizeof(WAVEFORMATEX);
                const size_t i_incboff = ( p_wf->wFormatTag == WAVE_FORMAT_EXTENSIBLE ) ?
                                          sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX): 0;
                if( p_strf->i_chunk_size >= i_cboff + p_wf->cbSize &&
                    p_wf->cbSize > i_incboff )
                {
                    int i_extra = p_wf->cbSize - i_incboff;
                    tk->fmt.p_extra = malloc( i_extra );
                    if( unlikely(tk->fmt.p_extra == NULL) )
                    {
                        es_format_Clean( &tk->fmt );
                        free( tk );
                        goto error;
                    }
                    tk->fmt.i_extra = i_extra;
                    memcpy( tk->fmt.p_extra, ((uint8_t *)(&p_wf[1])) + i_incboff, i_extra );
                }
                break;
            }

            case( AVIFOURCC_vids ):
            {
                if(unlikely( p_strf->i_cat != VIDEO_ES ))
                {
                    free(tk);
                    continue;
                }

                VLC_BITMAPINFOHEADER *p_bih = p_strf->u.p_bih;

                if( p_bih->biCompression == VLC_FOURCC( 'D', 'X', 'S', 'B' ) )
                {
                   msg_Dbg( p_demux, "stream[%u] subtitles", i );
                   es_format_Init( &tk->fmt, SPU_ES, p_bih->biCompression );
                   break;
                }

                es_format_Init( &tk->fmt, VIDEO_ES,
                        AVI_FourccGetCodec( VIDEO_ES, p_bih->biCompression ) );

                if( p_bih->biCompression == BI_RGB )
                {
                    switch( p_bih->biBitCount )
                    {
                        case 32:
                            tk->fmt.i_codec = VLC_CODEC_RGB32;
                            Set_BMP_RGB_Masks( &tk->fmt );
                            break;
                        case 24:
                            tk->fmt.i_codec = VLC_CODEC_RGB24; /* BGR (see biBitCount) */
                            Set_BMP_RGB_Masks( &tk->fmt );
                            break;
                        case 16: /* Yes it is RV15 */
                        case 15:
                            tk->fmt.i_codec = VLC_CODEC_RGB15;
                            Set_BMP_RGB_Masks( &tk->fmt );
                            break;
                        case 9: /* <- TODO check that */
                            tk->fmt.i_codec = VLC_CODEC_I410;
                            break;
                        case 8:
                            if ( p_bih->biClrUsed )
                                tk->fmt.i_codec = VLC_CODEC_RGBP;
                            else
                                tk->fmt.i_codec = VLC_CODEC_GREY;
                            break;
                    }

                    if( tk->fmt.i_codec == VLC_CODEC_RGBP )
                    {
                        tk->fmt.video.p_palette = malloc( sizeof(video_palette_t) );
                        if ( tk->fmt.video.p_palette )
                        {
                            const VLC_BITMAPINFO *p_bi = (const VLC_BITMAPINFO *)p_bih;
                            uint32_t entry;
                            for( uint32_t j = 0; j < p_bih->biClrUsed; j++ )
                            {
                                 entry = GetDWBE( &p_bi->bmiColors[j] );
                                 tk->fmt.video.p_palette->palette[j][0] = entry >> 24;
                                 tk->fmt.video.p_palette->palette[j][1] = (entry >> 16) & 0xFF;
                                 tk->fmt.video.p_palette->palette[j][2] = (entry >> 8) & 0xFF;
                                 tk->fmt.video.p_palette->palette[j][3] = entry & 0xFF;
                            }
                            tk->fmt.video.p_palette->i_entries = p_bih->biClrUsed;
                        }
                    }

                    tk->i_width_bytes = p_bih->biWidth * (p_bih->biBitCount >> 3);
                    /* RGB DIB are coded from bottom to top */
                    if ( p_bih->biHeight < INT32_MAX ) tk->b_flipped = true;
                }
                else
                {
                    tk->fmt.i_codec = p_bih->biCompression;
                    if( tk->fmt.i_codec == VLC_CODEC_MP4V &&
                        !strncasecmp( (char*)&p_strh->i_handler, "XVID", 4 ) )
                    {
                        tk->fmt.i_codec           =
                        tk->fmt.i_original_fourcc = VLC_FOURCC( 'X', 'V', 'I', 'D' );
                    }

                    /* Shitty files storing chroma in biCompression */
                    Set_BMP_RGB_Masks( &tk->fmt );
                }

                if( IsQNAPCodec( tk->fmt.i_codec ) )
                    tk->fmt.b_packetized = false;

                tk->i_samplesize = 0;

                tk->fmt.video.i_visible_width =
                tk->fmt.video.i_width  = p_bih->biWidth;
                tk->fmt.video.i_visible_height =
                tk->fmt.video.i_height = p_bih->biHeight;
                tk->fmt.video.i_bits_per_pixel = p_bih->biBitCount;
                tk->fmt.video.i_frame_rate = tk->i_rate;
                tk->fmt.video.i_frame_rate_base = tk->i_scale;

                 /* Uncompresse Bitmap or YUV, YUV being always topdown */
                if ( tk->fmt.video.i_height > INT32_MAX )
                    tk->fmt.video.i_height =
                        (unsigned int)(-(int)p_bih->biHeight);

                avi_chunk_vprp_t *p_vprp = AVI_ChunkFind( p_strl, AVIFOURCC_vprp, 0, false );
                if( p_vprp )
                {
                    uint32_t i_frame_aspect_ratio = p_vprp->i_frame_aspect_ratio;
                    if( p_vprp->i_video_format_token >= 1 &&
                        p_vprp->i_video_format_token <= 4 )
                        i_frame_aspect_ratio = 0x00040003;
                    tk->fmt.video.i_sar_num = ((i_frame_aspect_ratio >> 16) & 0xffff) *
                                              tk->fmt.video.i_height;
                    tk->fmt.video.i_sar_den = ((i_frame_aspect_ratio >>  0) & 0xffff) *
                                              tk->fmt.video.i_width;
                }
                /* Extradata is the remainder of the chunk less the BIH */
                if( p_strf->i_chunk_size <= INT_MAX - sizeof(VLC_BITMAPINFOHEADER) )
                {
                    int i_extra = p_strf->i_chunk_size - sizeof(VLC_BITMAPINFOHEADER);
                    if( i_extra > 0 )
                    {
                        tk->fmt.p_extra = malloc( i_extra );
                        if( unlikely(tk->fmt.p_extra == NULL) )
                        {
                            es_format_Clean( &tk->fmt );
                            free( tk );
                            goto error;
                        }
                        tk->fmt.i_extra = i_extra;
                        memcpy( tk->fmt.p_extra, &p_bih[1], tk->fmt.i_extra );
                    }
                }

                msg_Dbg( p_demux, "stream[%u] video(%4.4s) %"PRIu32"x%"PRIu32" %dbpp %ffps",
                         i, (char*)&p_bih->biCompression,
                         (uint32_t)p_bih->biWidth,
                         (uint32_t)p_bih->biHeight,
                         p_bih->biBitCount,
                         (float)tk->i_rate/(float)tk->i_scale );

                /* Extract palette from extradata if bpp <= 8 */
                if( tk->fmt.video.i_bits_per_pixel > 0 && tk->fmt.video.i_bits_per_pixel <= 8 )
                {
                    /* The palette should not be included in biSize, but come
                     * directly after BITMAPINFORHEADER in the BITMAPINFO structure */
                    if( tk->fmt.i_extra > 0 )
                    {
                        free( tk->fmt.video.p_palette );
                        tk->fmt.video.p_palette = calloc( 1, sizeof(video_palette_t) );
                        if( likely(tk->fmt.video.p_palette) )
                        {
                            const uint8_t *p_pal = tk->fmt.p_extra;
                            tk->fmt.video.p_palette->i_entries = __MIN(tk->fmt.i_extra/4, 256);
                            for( int k = 0; k < tk->fmt.video.p_palette->i_entries; k++ )
                            {
                                for( int j = 0; j < 4; j++ )
                                    tk->fmt.video.p_palette->palette[k][j] = p_pal[4*k+j];
                            }
                        }
                    }
                }
                break;
            }

            case( AVIFOURCC_txts):
                msg_Dbg( p_demux, "stream[%u] subtitle attachment", i );
                AVI_ExtractSubtitle( p_demux, i, p_strl, p_strn );
                free( tk );
                continue;

            case( AVIFOURCC_iavs):
            case( AVIFOURCC_ivas):
                msg_Dbg( p_demux, "stream[%u] iavs with handler %4.4s", i, (char *)&p_strh->i_handler );
                es_format_Init( &tk->fmt, VIDEO_ES, AVI_FourccGetCodec( VIDEO_ES, p_strh->i_handler ) );
                tk->i_samplesize = 0;
                tk->i_dv_audio_rate = tk->fmt.i_codec == VLC_CODEC_DV ? -1 : 0;

                tk->fmt.video.i_visible_width =
                tk->fmt.video.i_width  = p_avih->i_width;
                tk->fmt.video.i_visible_height =
                tk->fmt.video.i_height = p_avih->i_height;
                break;

            case( AVIFOURCC_mids):
                msg_Dbg( p_demux, "stream[%u] midi is UNSUPPORTED", i );
                /* fall through */

            default:
                msg_Warn( p_demux, "stream[%u] unknown type %4.4s", i, (char *)&p_strh->i_type );
                free( tk );
                continue;
        }
        if( p_strn && p_strn->p_str )
            tk->fmt.psz_description = FromACP( p_strn->p_str );
        tk->p_es = es_out_Add( p_demux->out, &tk->fmt );
        TAB_APPEND( p_sys->i_track, p_sys->track, tk );
    }

    if( p_sys->i_track <= 0 ) 
    {
        msg_Err( p_demux, "no valid track" );
        goto error;
    }

    i_do_index = var_InheritInteger( p_demux, "avi-index" );
    if( i_do_index == 1 ) /* Always fix */
    {
aviindex:
        if( p_sys->b_fastseekable ) //off by default in 3.0.15
        {
            AVI_IndexCreate( p_demux );
        }
        else if( p_sys->b_seekable )
        {
            AVI_IndexLoad( p_demux );
        }
        else
        {
            msg_Warn( p_demux, "cannot create index (unseekable stream)" );
        }
    }
    else if( p_sys->b_seekable )
    {
        AVI_IndexLoad( p_demux );
    }

    /* *** movie length in sec *** */
    p_sys->i_length = AVI_MovieGetLength( p_demux );

    /* Check the index completeness */
    unsigned int i_idx_totalframes = 0;
    for( unsigned int i = 0; i < p_sys->i_track; i++ )
    {
        const avi_track_t *tk = p_sys->track[i];
        if( tk->fmt.i_cat == VIDEO_ES && tk->idx.p_entry )
            i_idx_totalframes = __MAX(i_idx_totalframes, tk->idx.i_size);
    }
    if( i_idx_totalframes != p_avih->i_totalframes &&
        p_sys->i_length < (mtime_t)p_avih->i_totalframes *
                          (mtime_t)p_avih->i_microsecperframe /
                          CLOCK_FREQ )
    {
        msg_Warn( p_demux, "broken or missing index, 'seek' will be "
                           "approximative or will exhibit strange behavior" );
        if( (i_do_index == 0 || i_do_index == 3) && !b_index )
        {
            if( !p_sys->b_fastseekable ) {
                b_index = true;
                goto aviindex;
            }
            if( i_do_index == 0 )
            {
                const char *psz_msg = _(
                    "Because this file index is broken or missing, "
                    "seeking will not work correctly.\n"
                    "VLC won't repair your file but can temporary fix this "
                    "problem by building an index in memory.\n"
                    "This step might take a long time on a large file.\n"
                    "What do you want to do?");
                switch( vlc_dialog_wait_question( p_demux,
                                                  VLC_DIALOG_QUESTION_NORMAL,
                                                  _("Do not play"),
                                                  _("Build index then play"),
                                                  _("Play as is"),
                                                  _("Broken or missing Index"),
                                                  "%s", psz_msg ) )
                {
                    case 0:
                        b_aborted = true;
                        goto error;
                    case 1:
                        b_index = true;
                        msg_Dbg( p_demux, "Fixing AVI index" );
                        goto aviindex;
                }
            }
            else
            {
                b_index = true;
                msg_Dbg( p_demux, "Fixing AVI index" );
                goto aviindex;
            }
        }
    }

    /* fix some BeOS MediaKit generated file */
    for( unsigned i = 0 ; i < p_sys->i_track; i++ )
    {
        avi_track_t         *tk = p_sys->track[i];

        if( tk->fmt.i_cat != AUDIO_ES ||
            tk->idx.i_size < 1 ||
            tk->i_scale != 1 ||
            tk->i_samplesize != 0 )
            continue;
        
        avi_chunk_list_t *p_strl = AVI_ChunkFind( p_hdrl, AVIFOURCC_strl, i, true );
        //subsequently changed to: avi_chunk_list_t *p_strl = AVI_ChunkFind( p_hdrl, AVIFOURCC_strl, tk->fmt.i_id, true );
    
        avi_chunk_strf_t *p_strf = AVI_ChunkFind( p_strl, AVIFOURCC_strf, 0, false );
        if( !p_strf || p_strf->i_cat != AUDIO_ES )
            continue;

        const WAVEFORMATEX *p_wf = p_strf->u.p_wf;
        if( p_wf->wFormatTag != WAVE_FORMAT_PCM &&
            tk->i_rate == p_wf->nSamplesPerSec )
        {
            int64_t i_track_length =
                tk->idx.p_entry[tk->idx.i_size-1].i_length +
                tk->idx.p_entry[tk->idx.i_size-1].i_lengthtotal;
            mtime_t i_length = (mtime_t)p_avih->i_totalframes *
                               (mtime_t)p_avih->i_microsecperframe;

            if( i_length == 0 )
            {
                msg_Warn( p_demux, "track[%u] cannot be fixed (BeOS MediaKit generated)", i );
                continue;
            }
            tk->i_samplesize = 1;
            tk->i_rate       = i_track_length  * CLOCK_FREQ / i_length;
            msg_Warn( p_demux, "track[%u] fixed with rate=%u scale=%u (BeOS MediaKit generated)", i, tk->i_rate, tk->i_scale );
        }
    }

    if( p_sys->b_seekable )
    {
        /* we have read all chunk so go back to movi */
        if( vlc_stream_Seek( p_demux->s, p_movi->i_chunk_pos ) )
            goto error;
    }
    /* Skip movi header */
    if( vlc_stream_Read( p_demux->s, NULL, 12 ) < 12 )
        goto error;

    p_sys->i_movi_begin = p_movi->i_chunk_pos;
    return VLC_SUCCESS;
