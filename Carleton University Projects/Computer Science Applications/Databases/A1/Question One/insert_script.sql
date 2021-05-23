--request to create a cd table if none currently exists
CREATE TABLE IF NOT EXISTS cds(
      cd_id text primary key not null, --cd unique id
      title text NOT NULL, --title of CD
      artist text NOT NULL, --artist whose CD it is or "various artists"
      producer text default NULL,
      year integer,
      contributer text --student number who contirbuted the data
      );
	  
--request to create a songs table if none currently exists
CREATE TABLE IF NOT EXISTS songs(
      song_id integer primary key not null, --auto incrementing key
      title text NOT NULL, --title of song
      composer text NOT NULL, --person or persons who wrote the song
      cd_id text NOT NULL, --cd this song appears on
      track integer NOT NULL, --track number of the song
      contributer text --student number who contirbuted the data
      );

--start inserting into the database
BEGIN TRANSACTION;

--insert song data
INSERT OR REPLACE INTO cds(cd_id, title, artist, producer, year, contributer) VALUES ('101041125_AR_Album', 'Abbey Road', 'Michael Jackson', 'Quincy Jones, Michael Jackson', 1982, '101041125');
INSERT OR REPLACE INTO cds(cd_id, title, artist, producer, year, contributer) VALUES ('101041125_TMIL_Album', 'The Man I Love', 'Frank Sinatra', 'Milt Gabler', 1957, '101041125');

--insert songs from the first (Frank Sinatra's) CD
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Man I Love, The', 'George Gershwin, Ira Gershwin', '101041125_TMIL_Album', 1, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Please Be Kind', 'Sammy Cahn, Saul Chaplin', '101041125_TMIL_Album', 2, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Happiness is a Thing Called Joe', 'Harold Arlen, Yip Harburg', '101041125_TMIL_Album', 3, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('(Just One Way to Say) I Love You', 'Irving Berlin', '101041125_TMIL_Album', 4, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('That''s All', 'Alan Brandt, Bob Haymes', '101041125_TMIL_Album', 5, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Something Wonderful', 'Oscar Hammerstein II, Richard Rodgers', '101041125_TMIL_Album', 6, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('He''s My Guy', 'Gene de Paul, Don Raye', '101041125_TMIL_Album', 7, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Then I''ll Be Tired of You', 'Harburg, Arthur Schwartz', '101041125_TMIL_Album', 8, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('My Heart Stood Still', 'Lorenz Hart, Rodgers', '101041125_TMIL_Album', 9, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('If I Should Lose You', 'Ralph Rainger, Leo Robin', '101041125_TMIL_Album', 10, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('There Is No Greater Love', 'Isham Jones, Marty Symes', '101041125_TMIL_Album', 11, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Folks Who Live On the Hill, The', 'Hammerstein, Jerome Kern', '101041125_TMIL_Album', 12, '101041125');

--insert songs from the second (Michael Jackson's) CD
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Wanna Be Startin'' Somethin''', 'Michael Jackson', '101041125_AR_Album', 1, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Baby Be Mine', 'Rod Temperton', '101041125_AR_Album', 2, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Girl Is Mine, The', 'Michael Jackson', '101041125_AR_Album', 3, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Thriller', 'Rod Temperton', '101041125_AR_Album', 4, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Beat It', 'Michael Jackson', '101041125_AR_Album', 5, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Billie Jean', 'Michael Jackson', '101041125_AR_Album', 6, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Human Nature', 'Steve Porcaro, John Bettis', '101041125_AR_Album', 7, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('P.Y.T. (Pretty Young Thing)', 'James Ingram, Quincy Jones', '101041125_AR_Album', 8, '101041125');
INSERT OR REPLACE INTO songs(title, composer, cd_id, track, contributer) VALUES ('Lady In My Life, The', 'Rod Temperton', '101041125_AR_Album', 9, '101041125');

--Remove any possible duplicated entries, by only retaining the copy of each entry with the lowest song_id number (aka the oldest copy)
DELETE FROM songs WHERE song_id NOT IN (SELECT MIN(song_id) FROM songs GROUP BY title, composer, cd_id, track, contributer);

--end the insertions of database entries
END TRANSACTION;