-- Keep a log of any SQL queries you execute as you solve the mystery.

-- This query will give us a start with the information in description column
SELECT *
FROM crime_scene_reports
WHERE street = 'Humphrey Street' AND year = 2021 AND month = 7 AND day = 28;
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

-- Now we will take a look at what interviewee had to say
SELECT *
FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28 AND id in (161, 162, 163);
-- | 161 | Ruth    | 2021 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- | 162 | Eugene  | 2021 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's
-- bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- | 163 | Raymond | 2021 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket. |

-- This query will give us the earliest flight on the day after theft (based ob the infromation given by Raymond)
SELECT *
FROM flights
JOIN airports ON flights.destination_airport_id=airports.id
WHERE year = 2021 AND month = 7 AND day = 29
ORDER BY hour;
-- | id | origin_airport_id | destination_airport_id | year | month | day | hour | minute | id | abbreviation |              full_name              |     city      |
-- +----+-------------------+------------------------+------+-------+-----+------+--------+----+--------------+-------------------------------------+---------------+
-- | 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     | 4  | LGA          | LaGuardia Airport                   | New York City |

-- This query will give as a list of suspects based on license_plate from bakery security logs and flight 36 passeger's license_plate
SELECT DISTINCT flight_id, passengers.passport_number, seat, people.id AS passenger_id, name, phone_number, people.license_plate
    FROM passengers
        JOIN people ON passengers.passport_number = people.passport_number
        JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
            WHERE flight_id = 36 AND people.license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 and minute >= 15 and minute <=25);
-- +-----------+-----------------+------+--------------+--------+----------------+---------------+
-- | flight_id | passport_number | seat | passenger_id |  name  |  phone_number  | license_plate |
-- +-----------+-----------------+------+--------------+--------+----------------+---------------+
-- | 36        | 1695452385      | 3B   | 398010       | Sofia  | (130) 555-0289 | G412CB7       |
-- | 36        | 5773159633      | 4A   | 686048       | Bruce  | (367) 555-5533 | 94KL13X       |
-- | 36        | 8294398571      | 6C   | 560886       | Kelsey | (499) 555-9472 | 0NTHK55       |
-- | 36        | 8496433585      | 7B   | 467400       | Luca   | (389) 555-5198 | 4328GD8       |
-- +-----------+-----------------+------+--------------+--------+----------------+---------------+

-- Now we narrow down the list of suspects with the phone call information given by Raymond
SELECT DISTINCT flight_id, passengers.passport_number, seat, people.id AS passenger_id, name, phone_number, people.license_plate
    FROM passengers
        JOIN people ON passengers.passport_number = people.passport_number
        JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
        JOIN phone_calls ON phone_calls.caller = people.phone_number
            WHERE flight_id = 36
            AND people.license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 and minute >= 15 and minute <=25)
            AND people.phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 and duration < 60);

-- +-----------+-----------------+------+--------------+--------+----------------+---------------+
-- | flight_id | passport_number | seat | passenger_id |  name  |  phone_number  | license_plate |
-- +-----------+-----------------+------+--------------+--------+----------------+---------------+
-- | 36        | 1695452385      | 3B   | 398010       | Sofia  | (130) 555-0289 | G412CB7       |
-- | 36        | 5773159633      | 4A   | 686048       | Bruce  | (367) 555-5533 | 94KL13X       |
-- | 36        | 8294398571      | 6C   | 560886       | Kelsey | (499) 555-9472 | 0NTHK55       |
-- +-----------+-----------------+------+--------------+--------+----------------+---------------+

-- Now we narrow down the list of suspects with the atm information given by Eugene
SELECT DISTINCT flight_id, passengers.passport_number, seat, people.id AS passenger_id, name, phone_number, people.license_plate
    FROM passengers
        JOIN people ON passengers.passport_number = people.passport_number
        JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
        JOIN phone_calls ON phone_calls.caller = people.phone_number
            WHERE flight_id = 36
            AND people.license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 and minute >= 15 and minute <=25)
            AND people.phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 and duration < 60)
            AND people.name IN (SELECT people.name FROM atm_transactions JOIN bank_accounts ON atm_transactions.account_number = bank_accounts.account_number JOIN people ON bank_accounts.person_id = people.id WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw');
-- +-----------+-----------------+------+--------------+-------+----------------+---------------+
-- | flight_id | passport_number | seat | passenger_id | name  |  phone_number  | license_plate |
-- +-----------+-----------------+------+--------------+-------+----------------+---------------+
-- | 36        | 5773159633      | 4A   | 686048       | Bruce | (367) 555-5533 | 94KL13X       |
-- +-----------+-----------------+------+--------------+-------+----------------+---------------+

-- Bingo! Now we just have to check, whom did Bruce call after the theft
SELECT *
FROM phone_calls
JOIN people ON people.phone_number = phone_calls.receiver
WHERE year = 2021 AND month = 7 AND day = 28 and duration < 60 AND phone_calls.caller = '(367) 555-5533';