#include <curl/curl.h>

//gcc -Wall -o main main.c -lcurl
size_t write_data (void *ptr, size_t size, size_t nmemb, FILE *stream);

int main (int argc, char *argv[])
{

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        FILE *fp = fopen ("filename.txt", "wb");

        curl_easy_setopt (curl, CURLOPT_URL, "https://example.com/file.txt");
        curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt (curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform (curl);

        /* Sempre limpe após usar */
        curl_easy_cleanup (curl);

        fclose (fp);
    }
    return 0;
}

size_t write_data (void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite (ptr, size, nmemb, stream);
    return written;
}
