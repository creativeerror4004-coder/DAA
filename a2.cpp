#include <bits/stdc++.h>
#include <iomanip>
using namespace std;
using namespace std::chrono;


// --------------------- MOVIE STRUCTURE ---------------------
struct Movie {
    string title;
    float rating;
    int release_year;
    int popularity;


    void display() const {
        cout << title
             << " | Rating: " << rating
             << " | Year: " << release_year
             << " | Popularity: " << popularity << endl;
    }
};


// --------------------- QUICKSORT IMPLEMENTATION ---------------------
void quickSort(vector<Movie> &movies, int low, int high, bool (*compare)(const Movie &, const Movie &)) {
    if (low < high) {
        int pivotIndex = low;
        Movie pivot = movies[high];
        for (int i = low; i < high; i++) {
            if (compare(movies[i], pivot)) {
                swap(movies[i], movies[pivotIndex]);
                pivotIndex++;
            }
        }
        swap(movies[pivotIndex], movies[high]);


        quickSort(movies, low, pivotIndex - 1, compare);
        quickSort(movies, pivotIndex + 1, high, compare);
    }
}


// --------------------- COMPARATOR FUNCTIONS ---------------------
bool compareByRating(const Movie &a, const Movie &b) {
    return a.rating < b.rating;
}


bool compareByYear(const Movie &a, const Movie &b) {
    return a.release_year < b.release_year;
}


bool compareByPopularity(const Movie &a, const Movie &b) {
    return a.popularity < b.popularity;
}


// --------------------- RANDOM MOVIE GENERATOR ---------------------
vector<Movie> generateMovies(int n) {
    vector<Movie> movies;
    movies.reserve(n);
    for (int i = 0; i < n; i++) {
        Movie m;
        m.title = "Movie " + to_string(i + 1);
        m.rating = static_cast<float>(rand() % 90 + 10) / 10.0f;   // Ratings between 1.0 and 10.0
        m.release_year = rand() % 45 + 1980;                       // 1980 to 2024
        m.popularity = rand() % 1000000 + 1000;                    // 1,000 to 1,000,000
        movies.push_back(m);
    }
    return movies;
}


// --------------------- MAIN FUNCTION ---------------------
int main() {
    srand(time(0));

    int num_movies = 1000000; // Use 1,000,000 for stress testing
    cout << "Generating " << num_movies << " random movies..." << endl;


    vector<Movie> movies = generateMovies(num_movies);


    cout << "Sort movies by (rating/year/popularity): ";
    string sort_by;
    cin >> sort_by;


    bool (*compare)(const Movie &, const Movie &);
    if (sort_by == "rating")
        compare = compareByRating;
    else if (sort_by == "year")
        compare = compareByYear;
    else if (sort_by == "popularity")
        compare = compareByPopularity;
    else {
        cout << "Invalid choice. Defaulting to rating." << endl;
        compare = compareByRating;
    }


    cout << "Sorting movies using QuickSort..." << endl;
    auto start = high_resolution_clock::now();


    quickSort(movies, 0, movies.size() - 1, compare);


    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;


    cout << "Sorting completed in " << fixed << setprecision(3)
         << diff.count() << " seconds." << endl;


    // Display top 10 movies in descending order (best-ranked first)
    cout << "\nTop 10 Movies by " << sort_by << ":" << endl;
    for (int i = num_movies - 1; i >= num_movies - 10; --i)
        movies[i].display();


    return 0;
}
