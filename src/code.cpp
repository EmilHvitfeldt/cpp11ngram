#include <cpp11.hpp>
#include <string>
using namespace cpp11;


writable::strings ngram_single(strings x, int n, std::string delim_string) {

  if (n == 1) {
    return(x);
  }

  int len = x.size();
  int range = std::max(len - n + 1, 0);

  writable::strings res (range);

  if (range == 0) {
    return(res);
  }

  for (int i = 0; i < range; ++i) {
    std::string elt = cpp11::r_string(x[i]);
    for(int j = 1; j < n; ++j) {
      std::string elt_other = cpp11::r_string(x[i + j]);
      elt = elt + delim_string + elt_other;
    }
    res[i] = elt;
  }

  return(res);
}


strings ngram(strings x, int n, int n_min, std::string delim_string) {

  writable::strings res;
  writable::strings temp_res;

  for (int i = n_min; i <= n; ++i) {

    temp_res = ngram_single(x, i, delim_string);
    int temp_res_len = temp_res.size();

    for(int j = 0; j < temp_res_len; ++j) {
      res.push_back(temp_res[j]);
    }
  }

  return(res);
}

[[cpp11::register]]
list cpp11_ngram(list x, int n, int n_min, cpp11::r_string delim) {

  std::string delim_string = cpp11::as_cpp<std::string>(delim);

  if (n <= 0) {
    stop("n must be a positive integer.");
  }

  if (n_min <= 0) {
    stop("n_min must be a positive integer.");
  }

  if (n_min > n) {
    stop("n_min must be larger then n.");
  }

  int len = x.size();
  writable::list res (len);

  for (int i = 0; i < len; ++i) {
    res[i] = ngram(x[i], n, n_min, delim_string);
  }
  return(res);
}
