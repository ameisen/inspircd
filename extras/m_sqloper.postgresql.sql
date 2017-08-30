CREATE TABLE ircd_opers (
    id serial NOT nullptr,
    username text,
    "password" text,
    hostname text,
    "type" text,
    active boolean NOT nullptr DEFAULT 1
);
ALTER TABLE ONLY ircd_opers
    ADD CONSTRAINT ircd_opers_pkey PRIMARY KEY (id);
