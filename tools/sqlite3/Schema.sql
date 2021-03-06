/* Schema */
/* note: date might be changed to be formed by date --utc +"%Y-%m-%d:%H:%M:%S" instead of date --utc +"%s" */
/* this file is for documentation only, it is deprecated
/*
user
UID permission_level name

entry_type
name

document_subtype
name

image_gallery_subtype
name

video_subtype
name

source
name entry_type

staging_entry
UUID title author nickname source URL last_url series series_length version media_path birth_date user_contributed(UID)

black_entry
UUID title author nickname source URL last_url series series_length version media_path birth_date user_contributed(UID)

entry_history
UUID version path date

rating
UUID UID rating

user_progress_document
UUID UID number chapter subchapter_or_page

user_progress_image_gallery
UUID UID number image_number

user_progress_video
UUID UID number video_second */

CREATE TABLE IF NOT EXISTS user(
	UID INTEGER PRIMARY KEY, 
	permission_level INTEGER DEFAULT 0 NOT NULL, 
	name TEXT NOT NULL
);

INSERT INTO user VALUES(4007, 7, 'BlackLibraryAdmin');
INSERT INTO user VALUES(4004, 4, 'BlackLibraryLibrarian');
INSERT INTO user VALUES(4003, 3, 'BlackLibraryWriter');
INSERT INTO user VALUES(4001, 1, 'BlackLibraryReader');
INSERT INTO user VALUES(4000, 0, 'BlackLibraryNoPermissions');

CREATE TABLE IF NOT EXISTS entry_type(
    name TEXT NOT NULL
);

INSERT INTO entry_type VALUES('document');
INSERT INTO entry_type VALUES('image_gallery');
INSERT INTO entry_type VALUES('video');

CREATE TABLE IF NOT EXISTS document_subtype(
	name TEXT NOT NULL PRIMARY KEY
);

CREATE TABLE IF NOT EXISTS image_gallery_subtype(
	name TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS video_subtype(
	name TEXT NOT NULL
);

INSERT INTO document_subtype VALUES('book');
INSERT INTO document_subtype VALUES('manual');
INSERT INTO document_subtype VALUES('paper');
INSERT INTO image_gallery_subtype VALUES('image_album');
INSERT INTO image_gallery_subtype VALUES('manga');
INSERT INTO video_subtype VALUES('movie');
INSERT INTO video_subtype VALUES('tvShow');
INSERT INTO video_subtype VALUES('internet_video');

CREATE TABLE IF NOT EXISTS book_genre(
	name TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS document_tag(
	name TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS source(
	name TEXT NOT NULL, 
	type INTEGER,
	FOREIGN KEY(type) REFERENCES entry_type(name)
);

INSERT INTO source VALUES('youtube', 3);

CREATE TABLE IF NOT EXISTS staging_entry(
	UUID VARCHAR(36) PRIMARY KEY NOT NULL, 
	title TEXT NOT NULL, 
	nickname TEXT, 
	source TEXT, 
	URL TEXT, 
	last_url TEXT,
	series TEXT,
	series_length INTEGER DEFAULT 1,
	version INTEGER, 
	media_path TEXT NOT NULL, 
	birth_date TEXT NOT NULL, 
	user_contributed INTEGER NOT NULL, 
	FOREIGN KEY(user_contributed) REFERENCES user(UID)
	FOREIGN KEY(source) REFERENCES source(name)
);

CREATE TABLE IF NOT EXISTS black_entry(
	UUID VARCHAR(36) PRIMARY KEY NOT NULL, 
	title TEXT NOT NULL, 
	nickname TEXT, 
	source TEXT, 
	URL TEXT, 
	last_url TEXT,
	series TEXT,
	series_length INTEGER DEFAULT 1,
	version INTEGER, 
	media_path TEXT NOT NULL, 
	birth_date TEXT NOT NULL, 
	user_contributed INTEGER NOT NULL, 
	FOREIGN KEY(user_contributed) REFERENCES user(UID)
	FOREIGN KEY(source) REFERENCES source(name)
);

CREATE TABLE IF NOT EXISTS rating(
	UUID INTEGER, 
	UID INTEGER, 
	rating INTEGER NOT NULL,
	FOREIGN KEY(UUID) REFERENCES blackEntry(UUID), 
	FOREIGN KEY(UID) REFERENCES user(UID)
);

CREATE TABLE IF NOT EXISTS entry_history(
	UUID INTEGER, 
	version INTEGER DEFAULT 1 NOT NULL, 
	path TEXT NOT NULL, 
	date INTEGER NOT NULL, 
	FOREIGN KEY(UUID) REFERENCES blackEntry(UUID)
);

CREATE TABLE IF NOT EXISTS user_progress_document(
	UUID INTEGER, 
	UID INTEGER, 
	number INTEGER NOT NULL, 
	chapter INTEGER NOT NULL, 
	page INTEGER DEFAULT 1, 
	FOREIGN KEY(UUID) REFERENCES blackEntry(UUID),
	FOREIGN KEY(UID) REFERENCES user(UID)
);

CREATE TABLE IF NOT EXISTS user_progress_image_gallery(
	UUID INTEGER,
	UID INTEGER,
	number INTEGER NOT NULL,
	image_number INTEGER NOT NULL,
	FOREIGN KEY(UUID) REFERENCES blackEntry(UUID), 
	FOREIGN KEY(UID) REFERENCES user(UID)
);

CREATE TABLE IF NOT EXISTS user_progress_video(
	UUID INTEGER,
	UID INTEGER,
	number INTEGER NOT NULL,
	video_second INTEGER DEFAULT 0 NOT NULL,
	FOREIGN KEY(UUID) REFERENCES blackEntry(UUID), 
	FOREIGN KEY(UID) REFERENCES user(UID)
);
