SELECT DISTINCT name FROM people
    JOIN stars ON stars.person_id=people.id
    JOIN movies ON movies.id=stars.movie_id
    WHERE movies.id in (
        SELECT movies.id FROM movies
        JOIN stars ON stars.movie_id=movies.id
        JOIN people ON people.id=stars.person_id
        WHERE name = "Kevin Bacon" and birth = 1958)
    AND name != "Kevin Bacon";