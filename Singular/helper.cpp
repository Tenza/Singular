/*
 * Singular
 * Copyright (C) 2015 Filipe Carvalho
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "helper.h"

/**
 * @brief Helper::parse_json_object
 *      Recursive function that inserts into &listings, the keys and values that match &keys.
 *      This function with the help of 'parse_json_array' searches on every branch of the calling QJsonObject.
 * @param object
 *      The object from we extract the data.
 * @param keys
 *      The values that we want to extract.
 * @param listings
 *      The reference to insert the values.
 */
void Helper::parse_json_object(const QJsonObject &object, const QStringList &keys, QVariantHash &listings)
{
    QStringList listing_keys = object.keys();

    for(int i = 0; i < listing_keys.size(); i++)
    {
        if(object.value(listing_keys.at(i)).isObject())
        {
            parse_json_object(object.value(listing_keys.at(i)).toObject(), keys, listings);
        }
        else if(object.value(listing_keys.at(i)).isArray())
        {
            parse_json_array(object.value(listing_keys.at(i)).toArray(), keys, listings);
        }
        else
        {
            for(int j = 0; j < keys.size(); j++)
            {
                if(listing_keys.at(i) == keys.at(j))
                {
                    listings.insert(listing_keys.at(i), object.value(listing_keys.at(i)));
                    break;
                }
            }
        }
    }
}

/**
 * @brief Helper::parse_json_array
 *      Recursive function used to navigate JSON arrays. This function only navigates the array and assumes
 *      that the final value is always a key-value pair within an object. Only call this function directly, if
 *      your initial data is an array that contains objects with key-value pairs to save in &listings.
 * @param array
 *      The array to naviagate.
 * @param keys
 *  	The keys to be passed once an object or other array are found.
 * @param listings
 *      The reference to pass once an object or other array are found.
 */
void Helper::parse_json_array(const QJsonArray &array, const QStringList &keys, QVariantHash &listings)
{
    QJsonArray::const_iterator i;
    for(i = array.constBegin(); i != array.constEnd(); i++)
    {
        if((*i).isObject())
        {
            parse_json_object((*i).toObject(), keys, listings);
        }
        else if((*i).isArray())
        {
            parse_json_array((*i).toArray(), keys, listings);
        }
    }
}

/**
 * @brief Helper::price_converter
 *      Converts the price from and to int.
 * @param price
 *      Value to convert. Has to be striped from any symbols.
 * @return
 *      Value converted.
 * @remarks WARNING
 *      On windows, making the operation on a single step produces wrong results, THIS IS CRITICAL.
 */
int Helper::price_converter(const double price)
{
    double convert = price * 100.0;
    return static_cast<int>(convert);
}

double Helper::price_converter(const int price)
{
    double convert = static_cast<double>(price) / 100.0;
    return convert;
}

/**
 * @brief Helper::percentage
 *      Gets the percentage of a given number.
 * @param number
 *      The number corresponding to 100%.
 * @param percentage
 *      The percentage from 'number'.
 * @return
 *      The percentage of 'number'.
 */
int Helper::percentage(const int number, const int percentage)
{
    double percent = round((static_cast<double>(percentage) * static_cast<double>(number)) / 100.0);
    return static_cast<int>(percent);
}

double Helper::percentage(const double number, const int percentage)
{
    double percent = (static_cast<double>(percentage) * number) / 100.0;
    return percent;
}

/**
 * @brief Helper::substring
 *      Gets the text in between start and end.
 * @param text
 *      The text to search.
 * @param start
 *      Constant start the search
 * @param end
 *      Constant end the search
 * @return
 *      The found text.
 */
QString Helper::substring(const QString &text, const QString &start, const QString &end)
{
    int id_index = text.indexOf(start) + start.size();
    int id_length = text.indexOf(end, id_index) - id_index;

    return text.mid(id_index, id_length);
}

/**
 * @brief Helper::random_string
 *      Creates a pseudo random string.
 * @param string_length
 *      Length of the string.
 * @return
 *      The string.
 */
QString Helper::random_string(const int length)
{
    QString result;
    const QString possible_characters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    for(int i = 0; i < length; ++i)
    {
       int index = qrand() % possible_characters.length();
       QChar next_char = possible_characters.at(index);
       result.append(next_char);
    }

    return result;
}

/**
 * @brief Helper::random_number
 *      Creates a pseudo random number.
 * @param low
 *      Lowest generated value
 * @param high
 *      Highest generated value
 * @return
 *      The number
 */
int Helper::random_number(const int low, const int high)
{
    return qrand() % ((high + 1) - low) + low;
}

/**
 * @brief Helper::get_thread_id
 *      This function is used to get the number of the current thread.
 *      Dependes on calling function obviously.
 *      This is used for display purposes only.
 * @return
 *      A string with the hexadecimal value of the current thread.
 */
QString Helper::get_thread_id()
{
    return "0x" + QString::number(reinterpret_cast<quintptr>(QThread::currentThreadId()), 16);
}

/**
 * @brief Helper::proxy_type
 *      Returns the suported proxy types.
 * @param type
 *      Proxy type as text from the UI.
 * @return
 *      The correct format.
 */
QNetworkProxy::ProxyType Helper::proxy_type(const QString &type)
{
    QNetworkProxy::ProxyType proxy;

    if(type == "HttpProxy")
    {
        proxy = QNetworkProxy::HttpProxy;
    }
    else if(type == "Socks5Proxy")
    {
        proxy = QNetworkProxy::Socks5Proxy;
    }

    return proxy;
}

