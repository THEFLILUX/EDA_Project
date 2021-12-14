CREATE TABLE nyc
(
  gid serial NOT NULL,
  VendorID integer NOT NULL,
  lpep_pickup_datetime character varying(50),
  Lpep_dropoff_datetime character varying(50),
  Store_and_fwd_flag character varying(10),
  RateCodeID integer NOT NULL,
  Pickup_longitude double precision,
  Pickup_latitude double precision,
  Dropoff_longitude double precision,
  Dropoff_latitude double precision,
  Passenger_count integer NOT NULL,
  Trip_distance double precision,
  Fare_amount double precision,
  Extra double precision,
  MTA_tax double precision,
  Tip_amount double precision,
  Tolls_amount double precision,
  Ehail_fee character varying(10),
  improvement_surcharge double precision,
  Total_amount double precision,
  Payment_type integer,
  Trip_type integer,
  NothingHere character varying(10),
  NothingHere2 character varying(10),
  the_geom geometry,
  the_geom2 geometry,
  CONSTRAINT nyc_pkey PRIMARY KEY (gid),
  CONSTRAINT enforce_dims_the_geom CHECK (st_ndims(the_geom) = 2),
  CONSTRAINT enforce_geotype_geom CHECK (geometrytype(the_geom) = 'POINT'::text OR the_geom IS NULL),
  CONSTRAINT enforce_srid_the_geom CHECK (st_srid(the_geom) = 4326),
  CONSTRAINT enforce_dims_the_geom2 CHECK (st_ndims(the_geom2) = 2),
  CONSTRAINT enforce_geotype_geom2 CHECK (geometrytype(the_geom2) = 'POINT'::text OR the_geom2 IS NULL),
  CONSTRAINT enforce_srid_the_geom2 CHECK (st_srid(the_geom2) = 4327)
);

--Indexacion
CREATE INDEX nyc_the_geom_gist
  ON nyc
  USING gist
  (the_geom)
  
CREATE INDEX nyc_the_geom2_gist
  ON nyc
  USING gist
  (the_geom2);
  

--Importando datos
copy nyc(VendorID, lpep_pickup_datetime, Lpep_dropoff_datetime, Store_and_fwd_flag, RateCodeID, Pickup_longitude, Pickup_latitude, Dropoff_longitude, Dropoff_latitude, Passenger_count, Trip_distance, Fare_amount, Extra, MTA_tax, Tip_amount, Tolls_amount, Ehail_fee, improvement_surcharge, Total_amount, Payment_type, Trip_type, NothingHere, NothingHere2) FROM 'D:\Descargas\green_tripdata_2015-01.csv' DELIMITERS ',' CSV HEADER;


--Data geometrica
UPDATE nyc
SET the_geom = ST_GeomFromText('POINT(' || Pickup_longitude || ' ' || Pickup_latitude || ')',4326);

UPDATE nyc
SET the_geom2 = ST_GeomFromText('POINT(' || Dropoff_longitude || ' ' || Dropoff_latitude || ')',4327);


--Consultas
SELECT *
FROM nyc;

SELECT Count(*)
FROM nyc
WHERE nyc.the_geom && ST_MakeEnvelope( -73.91, 40.80, -73.90, 40.81, 4326);

SELECT Count(*)
FROM nyc
WHERE nyc.the_geom && ST_MakeEnvelope( -73.96003, 40.814, -73.960025, 40.815, 4326);

SELECT Count(*)
FROM nyc
WHERE nyc.the_geom && ST_MakeEnvelope( -73.91, 40.70, -73.90, 40.80, 4326);