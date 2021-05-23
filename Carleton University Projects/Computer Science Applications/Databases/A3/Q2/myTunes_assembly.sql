
DROP TABLE IF EXISTS cds;
DROP TABLE IF EXISTS songs;
DROP TABLE IF EXISTS temp_cds;
DROP TABLE IF EXISTS temp_songs;

BEGIN TRANSACTION;

CREATE TABLE temp_cds(
      cd_id text primary key not null, --cd unique id
      title text NOT NULL, --title of CD
      artist text NOT NULL, --artist whose CD it is or "various artists"
      producer text default NULL,
      year integer,
      contributer text --student number who contirbuted the data
      );
CREATE TABLE temp_songs(
      song_id integer primary key not null, --auto incrementing key
      title text NOT NULL, --title of song
      composer text NOT NULL, --person or persons who wrote the song
      cd_id text NOT NULL, --cd this song appears on
      track integer NOT NULL, --track number of the song
      contributer text --student number who contirbuted the data
      );
END TRANSACTION;


.read myTunes_Abdelghny.sql
INSERT OR REPLACE INTO temp_cds (cd_id, title, artist, producer, year, contributer) SELECT cd_id, title, artist, producer, year, contributer from cds;
INSERT OR REPLACE INTO temp_songs (song_id, title, composer, cd_id, track, contributer) SELECT song_id, title, composer, cd_id, track, contributer from songs;
DROP TABLE IF EXISTS cds;
DROP TABLE IF EXISTS songs;


.read myTunes_Ali.sql
INSERT OR REPLACE INTO temp_cds (cd_id, title, artist, producer, year, contributer) SELECT cd_id, title, artist, producer, year, contributer from cds;
INSERT OR REPLACE INTO temp_songs (song_id, title, composer, cd_id, track, contributer) SELECT song_id, title, composer, cd_id, track, contributer from songs;
DROP TABLE IF EXISTS cds;
DROP TABLE IF EXISTS songs;

.read myTunes_Fatemeh.sql
INSERT OR REPLACE INTO temp_cds (cd_id, title, artist, producer, year, contributer) SELECT cd_id, title, artist, producer, year, contributer from cds;
INSERT OR REPLACE INTO temp_songs (song_id, title, composer, cd_id, track, contributer) SELECT song_id, title, composer, cd_id, track, contributer from songs;
DROP TABLE IF EXISTS cds;
DROP TABLE IF EXISTS songs;

.read myTunes_Monica.sql
INSERT OR REPLACE INTO temp_cds (cd_id, title, artist, producer, year, contributer) SELECT cd_id, title, artist, producer, year, contributer from cds;
INSERT OR REPLACE INTO temp_songs (song_id, title, composer, cd_id, track, contributer) SELECT song_id, title, composer, cd_id, track, contributer from songs;
DROP TABLE IF EXISTS cds;
DROP TABLE IF EXISTS songs;

.read myTunes_Rezieh.sql
INSERT OR REPLACE INTO temp_cds (cd_id, title, artist, producer, year, contributer) SELECT cd_id, title, artist, producer, year, contributer from cds;
INSERT OR REPLACE INTO temp_songs (song_id, title, composer, cd_id, track, contributer) SELECT song_id, title, composer, cd_id, track, contributer from songs;
DROP TABLE IF EXISTS cds;
DROP TABLE IF EXISTS songs;

.read myTunes_Saman.sql
INSERT OR REPLACE INTO temp_cds (cd_id, title, artist, producer, year, contributer) SELECT cd_id, title, artist, producer, year, contributer from cds;
INSERT OR REPLACE INTO temp_songs (song_id, title, composer, cd_id, track, contributer) SELECT song_id, title, composer, cd_id, track, contributer from songs;
DROP TABLE IF EXISTS cds;
DROP TABLE IF EXISTS songs;

BEGIN TRANSACTION;

CREATE TABLE cds(
      cd_id text primary key not null, --cd unique id
      title text NOT NULL, --title of CD
      artist text NOT NULL, --artist whose CD it is or "various artists"
      producer text default NULL,
      year integer,
      contributer text --student number who contirbuted the data
      );
CREATE TABLE songs(
      song_id integer primary key not null, --auto incrementing key
      title text NOT NULL, --title of song
      composer text NOT NULL, --person or persons who wrote the song
      cd_id text NOT NULL, --cd this song appears on
      track integer NOT NULL, --track number of the song
      contributer text --student number who contirbuted the data
      );
END TRANSACTION;

INSERT OR REPLACE INTO cds (cd_id, title, artist, producer, year, contributer) SELECT cd_id, title, artist, producer, year, contributer from temp_cds;
INSERT OR REPLACE INTO songs (song_id, title, composer, cd_id, track, contributer) SELECT song_id, title, composer, cd_id, track, contributer from temp_songs;

DROP TABLE IF EXISTS temp_cds;
DROP TABLE IF EXISTS temp_songs;