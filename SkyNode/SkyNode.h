
#ifndef OSGEARTHUTIL_SKY_NODE
#define OSGEARTHUTIL_SKY_NODE
#include <windows.h>
#include "Utils.h"
#include "StarData.h"
#include <osg/Referenced>
#include <osg/MatrixTransform>
#include <osg/Uniform>
#include <osg/Group>
#include <osg/View>
#include <osgDB/ReadFile>
#include <iostream>
#include <iosfwd>
#include<string>
using std::string;
namespace osgEarth { namespace Util 
{
    using namespace osgEarth;
	template<typename T> inline T
		as( const std::string& str, const T& default_value )
	{
		T temp = default_value;
		std::istringstream strin( str );
		if ( !strin.eof() ) strin >> temp;
		return temp;
	}

    /**
    * Class that provides information about astronomical objects at a given time.
    */
    class __declspec(dllexport) EphemerisProvider : public osg::Referenced
    {
    public:
        /**
        * Gets the moon position in geocentric coordinates at the given time
        */
        virtual osg::Vec3d getMoonPosition( int year, int month, int date, double hoursUTC ) = 0;

        /**
        * Gets the sun position in geocentric coordinates at the given time
        */
        virtual osg::Vec3d getSunPosition( int year, int month, int date, double hoursUTC ) = 0;
    };


    /**
    * The default EphemerisProvider, provides positions based on freely available models
    */
    class __declspec(dllexport) DefaultEphemerisProvider : public EphemerisProvider
    {
    public:
        /**
        * Gets the moon position in geocentric coordinates at the given time
        */
        virtual osg::Vec3d getMoonPosition( int year, int month, int date, double hoursUTC );

        /**
        * Gets the sun position in geocentric coordinates at the given time
        */
        virtual osg::Vec3d getSunPosition( int year, int month, int date, double hoursUTC );
    };


    /**
     * A sky model.
     */
    class __declspec(dllexport) SkyNode : public osg::Group
    {
    public:
        /** Creates a new sky node based on the provided map. */
        SkyNode( const std::string& starFile="", float minStarMagnitude=-1.0f );
        SkyNode( float minStarMagnitude );

        /** dtor */
        virtual ~SkyNode() { }


      
    public:
        /**
         * Gets/Sets the EphemerisProvider
         */
        EphemerisProvider* getEphemerisProvider() const;
        void setEphemerisProvider(EphemerisProvider* ephemerisProvider );

        /**
         * Gets a position from the right ascension, declination and range
         * @param ra
         *        Right ascension in radians
         * @param decl
         *        Declination in radians
         * @param range
         *        Range in meters
         */
        static osg::Vec3d getPositionFromRADecl( double ra, double decl, double range );

    public:

        /** Attached this sky node to a view (placing a sky light). */
        void attach( osg::View* view, int lightNum =0 );
       
        /** Gets the date time for the sky position  */
        void getDateTime( int &year, int &month, int &date, double &hoursUTC, osg::View* view=0L );

        /** Sets the sky's position based on a julian date. */
        void setDateTime( int year, int month, int date, double hoursUTC, osg::View* view =0L );

        /** The minimum brightness for non-sunlit areas. */
        void setAmbientBrightness( float value, osg::View* view =0L );
        float getAmbientBrightness( osg::View* view =0L ) const;

        /** Enables or disables automatic ambience */
        void setAutoAmbience(bool value);
        bool getAutoAmbience() const;

        /** Whether the moon is visible */
        void setMoonVisible( bool value, osg::View* view =0L );
        bool getMoonVisible( osg::View* view =0L ) const;

        /** Whether the stars are visible */
        void setStarsVisible( bool value, osg::View* view =0L );
        bool getStarsVisible( osg::View* view =0L ) const;
		osg::Node* getSun(){return _sun;}
    public:
        //override
        virtual void traverse( osg::NodeVisitor& nv );

        //override
        virtual osg::BoundingSphere computeBound() const;

    private:

        /** Sets the sun's position as a unit vector. */
        void setSunPosition( const osg::Vec3& pos, osg::View* view =0L );

        /** Sets the moon position as a geocentric coordinate */
        void setMoonPosition( const osg::Vec3d& pos, osg::View* view =0L );

        /** Sets the sun's position as a latitude and longitude. */         
        void setSunPosition( double lat_degrees, double lon_degrees, osg::View* view =0L );        


        struct StarData
        {
            std::string name;
            double right_ascension;
            double declination;
            double magnitude;
            
            StarData() { }
            StarData( std::stringstream &ss );
        };

        struct PerViewData
        {
            osg::Vec3f                         _lightPos;
            osg::ref_ptr<osg::Light>           _light;
            osg::ref_ptr<osg::Uniform>         _lightPosUniform;
            osg::Matrixd                       _sunMatrix;            
            osg::Matrixd                       _moonMatrix;            
            osg::Matrixd                       _starsMatrix;
            bool                               _starsVisible;
            bool                               _moonVisible;

            // only available in per-view structures..not default
            osg::ref_ptr<osg::Group>           _cullContainer;
            osg::ref_ptr<osg::MatrixTransform> _sunXform;
            osg::ref_ptr<osg::MatrixTransform> _moonXform;
            osg::ref_ptr<osg::MatrixTransform> _starsXform;
            
            int _year;
            int _month;
            int _date;
            double _hoursUTC;
        };

        PerViewData _defaultPerViewData;
        typedef std::map<osg::View*, PerViewData> PerViewDataMap;
        PerViewDataMap _perViewData;

        float _innerRadius, _outerRadius, _sunDistance, _starRadius, _minStarMagnitude;
        osg::ref_ptr<osg::Node> _sun, _stars, _atmosphere, _moon;
        osg::ref_ptr<osg::Uniform> _starAlpha;
        osg::ref_ptr<osg::Uniform> _starPointSize;

        osg::Vec3d _moonPosition;
        bool _autoAmbience;

        osg::ref_ptr< const osg::EllipsoidModel > _ellipsoidModel;

        void initialize( const std::string& starFile="" );

        void makeAtmosphere( const osg::EllipsoidModel* );
        void makeSun();
        void makeMoon();

        void makeStars(const std::string& starFile);
        osg::Node* buildStarGeometry(const std::vector<StarData>& stars);
        void getDefaultStars(std::vector<StarData>& out_stars);
        bool parseStarFile(const std::string& starFile, std::vector<StarData>& out_stars);

        void setAmbientBrightness( PerViewData& data, float value );
        void setSunPosition( PerViewData& data, const osg::Vec3& pos );
        void setMoonPosition( PerViewData& data, const osg::Vec3d& pos );

        osg::ref_ptr< EphemerisProvider > _ephemerisProvider;
    };

} } // namespace osgEarth::Util

#endif //OSGEARTHUTIL_SKY_NODE
