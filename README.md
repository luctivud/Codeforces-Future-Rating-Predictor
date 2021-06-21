# Codeforces-Future-Rating-Predictor

Currently, it works on a statistical approach with a few randomisation techniques and outlier management technique. A modified form of K-means clustering has been used to detect the upper bound of rating and a free-form mean is used to get the lower bound of the rating. 

Currently just two clusters are used and the data is modified based on medians and random seeds. The actual rating prediction mechanism is based on the values provided by the seeds. 

K-Means employed rating is given at least 2x more preference than the normal mean employed rating, and the final rating is calculated.

Currently, the model needs the following input from the user:<br>
- max_rating <br>
- current_rating <br>
- ratings_change1 <br>
- ratings_change2 <br>
- ratings_change3 <br>
- lucky_number <br>


#### Future Development: 

To make it work under a Machine Learning model with the help of Codeforces API. 

Link to the codeforces post: https://codeforces.com/blog/entry/92017
