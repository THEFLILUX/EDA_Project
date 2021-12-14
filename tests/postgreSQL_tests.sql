CREATE TABLE(
    VendorID text, lpep_pickup_datetime text, Lpep_dropoff_datetime text, Store_and_fwd_flag text, RateCodeID text,
    a float, b float,
    Dropoff_longitude text, Dropoff_latitude text, Passenger_count text, Trip_distance text, Fare_amount text, Extra text,
    MTA_tax text, Tip_amount text, Tolls_amount text, Ehail_fee text, improvement_surcharge text, Total_amount text,
    Payment_type text, Trip_type text, campo text, camp text
);

\copy test FROM 'C:\Users\Jorge\OneDrive\Escritorio\test.csv' DELIMITER ',' CSV HEADER

query(-73.91, 40.80), Point(-73.90, 40.81);
query(-73.96003, 40.814), Point(-73.960025, 40.815);
query(-73.91, 40.70), Point(-73.90, 40.80);

SELECT * FROM test WHERE
a > -73.91 and a < -73.90 and
b > 40.80 and b < 40.81

SELECT * FROM test WHERE
a > -73.96003 and a < -73.960025 and
b > 40.814 and b < 40.815

SELECT * FROM test WHERE
a > -73.91 and a < -73.90 and
b > 40.70 and b < 40.80