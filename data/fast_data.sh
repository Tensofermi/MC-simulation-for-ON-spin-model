#### easy to process data.

cd script

# #### format the raw data in jobs or test
python3 1_format_jobs.py

# #### collect all the formatted data
python3 2_collect_data.py

# #### decompose the data in terms of specified parameters
python3 3_decom_data_L.py ../raw/raw.dat
python3 3_decom_data_beta.py ../raw/raw.dat

# #### compress the data in terms of different seeds
python3 4_compress_data.py ../raw/L_* ../raw/beta_*
