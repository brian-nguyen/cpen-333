import billboard
import json

# downloads the billboard chart from a billboard url lug (i.e. https://www.billboard.com/charts/lug)
#   and writes the results to a file
def download_chart(lug, file):
    chart = billboard.ChartData(lug)
    songs = []
    for entry in chart:
        songs.append({'title': entry.title, 'artist': entry.artist})

    jj_str = json.dumps(songs, sort_keys=True, indent=4)

    ff = open(file, 'w')
    ff.write(jj_str)
    ff.close()


# download a bunch of charts
download_chart('hot-100', 'billboard_hot_100.json')
download_chart('greatest-hot-100-singles', 'billboard_greatest_hot_100.json')
download_chart('adult-pop-songs', 'billboard_adult_pop.json')
download_chart('adult-contemporary', 'billboard_adult_contemporary.json')
download_chart('country-songs', 'billboard_country.json')
download_chart('rock-songs', 'billboard_rock.json')
download_chart('alternative-songs', 'billboard_alternative.json')
download_chart('rap-song', 'billboard_rap.json')
download_chart('r-and-b-songs', 'billboard_r&b.json')
download_chart('dance-electronic-songs', 'billboard_electronic.json')
download_chart('jazz-songs', 'billboard_jazz.json')