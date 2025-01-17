import java.util.Collection;
import java.util.Comparator;
import java.util.Iterator;
import java.util.TreeMap;

class Country implements Comparable<Country>{
    String name;
    double gdp2023;
    long population;
    public Country(String name, double gdp2023, long population){
        this.name = name;
        this.gdp2023 = gdp2023;
        this.population = population;
    }
    public String toString(){
        return String.format("Country : name="+name+", gdp2023=%.2f , population=%d", gdp2023, population);
    }
    public int compareTo(Country c){
        return Double.compare(this.gdp2023, c.gdp2023);
    }
}

public class T2_3 {
    public static void main(String[] args) {
        Country china = new Country("China",
                17732000, 1444216107);
        Country usa = new Country("USA",
                25487450, 331893745);
        Country germany = new Country("Germany",
                4242570, 83166711);
        Country france = new Country("France",
                2938270, 65386336);
        Country japan = new Country("Japan",
                4378943, 125681593);
        Country uk = new Country("UK",
                3257271, 68497913);
        Country italy = new Country("Italy",
                2116935, 60244639);
        Country southKorea = new Country("South Korea",
                1808160, 51635256);
        Country india = new Country("India",
                3766400, 1407563845);
        Country thailand = new Country("Thailand",
                540000, 71378000);
        System.out.println("Comparable:");
        TreeMap<Country,String> tm=new TreeMap<Country,String>();
        tm.put(china,"China");
        tm.put(usa,"USA");
        tm.put(germany,"Germany");
        tm.put(france,"France");
        tm.put(japan,"Japan");
        tm.put(uk,"UK");
        tm.put(italy,"Italy");
        tm.put(southKorea,"South Korea");
        tm.put(india,"India");
        tm.put(thailand,"Thailand");
        for(Country country : tm.keySet()){
            System.out.println(country);
        }

        System.out.println("Comparator:");
        TreeMap<Country ,String >tm2=new TreeMap<Country ,String >
                (Comparator.comparingDouble(c->c.gdp2023) );
        tm2.put(china,"China");
        tm2.put(usa,"USA");
        tm2.put(germany,"Germany");
        tm2.put(france,"France");
        tm2.put(japan,"Japan");
        tm2.put(uk,"UK");
        tm2.put(italy,"Italy");
        tm2.put(southKorea,"South Korea");
        tm2.put(india,"India");
        tm2.put(thailand,"Thailand");
        for(Country country : tm.keySet()){
            System.out.println(country);
        }

    }
}
