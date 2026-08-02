# zoology

Generic schema resolution, canonicalization, and validation for Roo data.

Union references can narrow the available variants at the use site:

```clojure
{:type :array
 :items {:ref :placement-rule
         :variants [:distance-range :inside-area]}}
```

The same whitelist can be written explicitly:

```clojure
{:ref :placement-rule
 :variants {:only [:distance-range :inside-area]}}
```

or as an exclusion from the referenced union:

```clojure
{:ref :placement-rule
 :variants {:except [:preferred-position :edge]}}
```

The referenced union remains canonical. The resolved use-site schema contains
only the requested variants, and the discriminator field's valid values are
narrowed to match.
